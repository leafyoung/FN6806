#!/usr/bin/env bash
#
# smoke_test.sh - build and run every module in this repo.
#
# A module passes if it compiles and then runs to completion without crashing.
# Program output is never checked for correctness, only for the absence of a
# crash. Compiler warnings are reported but never fatal: this repo builds with
# -Wall -Wextra and *without* -Werror, because some modules demonstrate
# warning-worthy code on purpose (60-exception's narrowing conversion,
# 96-test-xtensor-eigen's missing return).
#
# There is no top-level build system here - each module is compiled
# independently, exactly as AGENTS.md describes, and this script automates that
# per-module loop.
#
# Human-readable results go to stdout; a machine-readable record of every module
# goes to $SMOKE_DIR/results.jsonl (one JSON object per line).

set -uo pipefail

usage() {
  cat <<'USAGE'
Usage: ./smoke_test.sh [options] [filter...]

  filter...        only test modules whose directory name contains a filter
                   (e.g. `./smoke_test.sh 54-thread 82`)

Options:
  --build-only     compile only, skip the run phase
  --verbose, -v    echo each program's captured output
  --timeout SECS   default per-program run timeout (default 90)
  --keep, -k       keep run sandboxes after a successful pass
  --wasm           build/run for wasm32-wasip1 with the CPPBox classroom
                   toolchain instead of the host compiler
  --help, -h       show this help

Environment overrides:
  CXX              compiler (default clang++)
  CXXFLAGS         base compile flags (per-module profiles still apply)
  SMOKE_DIR        scratch directory for binaries, logs, sandboxes
  CPPBOX_ROOT      CPPBox checkout (default ~/devv/fin/classroom)
  WASI_SDK         wasi-sdk root (default: newest under
                   $CPPBOX_ROOT/wasi-toolchain/wasi-sdk-*)
  WASM_RUNNER      wasmtime | node (default: wasmtime if on PATH, else node)
USAGE
}

# --- Configuration -----------------------------------------------------------

REPO_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
readonly REPO_ROOT

CXX="${CXX:-clang++}"
# Base profile from AGENTS.md. -pthread is passed to every module: it is
# harmless for single-threaded code and required by 52-mc_gbm, 54-thread,
# 55-thread-atomic, 56-thread-struct, 71-multithread_mc_pi, 72-thread-mtx-cv,
# and 73-thread-local-prng.
CXXFLAGS="${CXXFLAGS:--Wall -Wextra -std=c++17 -pthread}"

SMOKE_DIR="${SMOKE_DIR:-${TMPDIR:-/tmp}/fn6806-smoke}"

RUN_TIMEOUT=90
TARGET=host
BUILD_ONLY=0
VERBOSE=0
KEEP=0
FILTERS=()

# Directories that are not modules.
readonly NON_MODULE_DIRS="third_party"

# No module in this repo is intentionally uncompilable (unlike FN6805's
# 30-header-file and 72-multiple_inclusion). The mechanism is kept so a future
# teaching example can be listed here and asserted to keep failing.
EXPECTED_BUILD_FAILURES=()

# --- Per-module profiles -----------------------------------------------------
#
# Case statements rather than associative arrays: bash 3.2 (stock macOS) has no
# `declare -A`, and a case arm reads as a small declarative table anyway.

# Full flag replacement, not append, so the effective command line for any
# module is visible in one place.
# The language standard and the include path a module needs are declared once
# here and consumed by both the host and the wasm builds, so the two targets
# cannot drift apart.
std_for_module() {
  case "$1" in
    # C++20 required: the `concept` keyword, and the module's own name says so.
    49d-variant_visit_cxx20 | 81-concept) echo "c++20" ;;
    *) echo "c++17" ;;
  esac
}

# -isystem, not -I: vendored Eigen/xtensor headers should not emit warnings for
# code we do not own. Their includes are spelled <eigen3/Eigen/Dense> and
# <xtensor/xarray.hpp>, both of which resolve under third_party/.
includes_for_module() {
  case "$1" in
    96-test-xtensor-eigen) echo "-isystem $REPO_ROOT/third_party" ;;
    *) echo "" ;;
  esac
}

# Full flag replacement, not append, so the effective command line for any
# module is visible in one place.
flags_for_module() {
  echo "${CXXFLAGS/-std=c++17/-std=$(std_for_module "$1")} $(includes_for_module "$1")"
}

# Most modules keep every source at their top level. 82-system splits an
# observer implementation into a subdirectory and lists it in its own Makefile.
find_depth_for_module() {
  case "$1" in
    82-system) echo "" ;; # unlimited: pick up observer/*.cpp
    *) echo "-maxdepth 1" ;;
  esac
}

# Per-module run limits. 92-et-vec-benchmark runs two 100-million-iteration
# loops with no optimisation flags and needs ~63s on a fast machine.
timeout_for_module() {
  case "$1" in
    92-et-vec-benchmark) echo 240 ;;
    *) echo "$RUN_TIMEOUT" ;;
  esac
}

# --- wasm target (CPPBox classroom parity) -----------------------------------
#
# The CPPBox teaching IDE (~/devv/fin/classroom) compiles and runs student code
# against wasm32-wasip1 with a bundled wasi-sdk and an embedded wasmtime, and
# falls back to podman only for code it cannot run that way. `--wasm` reproduces
# that path so this repo can be validated against the environment students
# actually use.

CPPBOX_ROOT="${CPPBOX_ROOT:-$HOME/devv/fin/classroom}"

# Flags copied from cppbox-core/src/wasi_exec.rs::compile. Each one is load
# bearing there, and the comments explaining why live in that file:
#   -fwasm-exceptions + the two -mllvm flags   non-legacy wasm EH
#   -Wl,--initial-memory / --max-memory        memory limits
#   -lunwind                                   unwinder for wasm EH
#   -lc-printscan-long-double                  wasi-libc trims long double
#                                              printf/scanf by default
#   -Wl,-z,stack-size                          see WASM_STACK_SIZE below
#
# WASM_STACK_SIZE is the one addition to CPPBox's own flag list. wasi-sdk
# defaults the wasm stack to 64 KiB, so an ordinary local array overflows it and
# traps with "memory access out of bounds". 8 MiB matches the usual host
# default. CPPBox should pass the same flag; until it does, this repo's wasm
# runs are slightly more forgiving than the classroom's.
WASM_STACK_SIZE="${WASM_STACK_SIZE:-8388608}"
readonly WASM_TARGET_FLAGS="--target=wasm32-wasip1 -O2 -Wall -Wextra \
-fwasm-exceptions -mllvm -wasm-enable-eh -mllvm -wasm-use-legacy-eh=false \
-Wl,--initial-memory=67108864 -Wl,--max-memory=4294967296 \
-Wl,-z,stack-size=$WASM_STACK_SIZE \
-lunwind -lc-printscan-long-double"

# CPPBox routes any project textually mentioning one of these headers to podman,
# because wasm32-wasip1-threads is non-functional upstream (see
# wasi_exec.rs::uses_threading). Matching that list exactly keeps this test
# honest: a module skipped here is a module CPPBox would not run on wasm either.
# Verified: 54-thread built for wasm traps with a WebAssembly.Exception as soon
# as std::thread is constructed.
readonly WASM_THREAD_MARKERS='<thread>|<future>|<mutex>|<condition_variable>|<atomic>|<shared_mutex>'

# Headers wasi-sdk's libc++ cannot serve at all, beyond CPPBox's own list.
# <execution> brings in the parallel algorithms (std::execution::par), which
# need threads and are absent from the wasm sysroot. No module here uses it
# today; FN6805's 52-stl does. CPPBox's uses_threading() does not list
# <execution>, so such code currently reaches wasm and fails to compile instead
# of being routed to podman.
readonly WASM_UNSUPPORTED_MARKERS='<execution>'

# Known wasm incompatibilities, reported as XFAIL-WASM with the reason so the
# suite stays green while the limitation stays visible.
# Empty today: every non-threaded module builds and runs on wasm.
#
# 47-poly_type used to belong here - K.h's Ksub allocated vector<int>(1e9) =
# 4 GB, which cannot fit wasm32's 4 GiB address space and threw std::bad_alloc
# while succeeding on Linux through overcommit. The allocation is now 100'000'000
# (400 MB) and fits, so the entry is gone.
wasm_expected_failure_reason() {
  case "$1" in
    *) echo "" ;;
  esac
}

WASM_CXX=""
WASM_SYSROOT=""
resolve_wasm_toolchain() {
  local sdk="${WASI_SDK:-}"
  if [[ -z "$sdk" ]]; then
    sdk="$(find "$CPPBOX_ROOT/wasi-toolchain" -maxdepth 1 -type d -name 'wasi-sdk-*' 2>/dev/null | sort -V | tail -1)"
  fi
  if [[ -z "$sdk" || ! -x "$sdk/bin/clang++" ]]; then
    echo "smoke_test.sh: no wasi-sdk found (looked under $CPPBOX_ROOT/wasi-toolchain)." >&2
    echo "  Set WASI_SDK=/path/to/wasi-sdk-NN.0, or CPPBOX_ROOT to your CPPBox checkout." >&2
    return 1
  fi
  WASM_CXX="$sdk/bin/clang++"
  WASM_SYSROOT="$sdk/share/wasi-sysroot"
  [[ -d "$WASM_SYSROOT" ]] || { echo "smoke_test.sh: missing sysroot $WASM_SYSROOT" >&2; return 1; }
}

# CPPBox embeds wasmtime as a library; the closest shell equivalent is the
# wasmtime CLI. Node ships a WASI preview1 implementation and is used when
# wasmtime is absent - the same ABI, a different host.
WASM_RUN_KIND=""
WASM_NODE_SHIM=""
WASMTIME_BIN=""
resolve_wasm_runner() {
  local want="${WASM_RUNNER:-}"
  if [[ "$want" == "node" ]] || { [[ -z "$want" ]] && ! command -v wasmtime >/dev/null 2>&1; }; then
    command -v node >/dev/null 2>&1 || { WASM_RUN_KIND=""; return 0; }
    WASM_RUN_KIND="node"
    WASM_NODE_SHIM="$SMOKE_DIR/wasi_run.mjs"
    cat >"$WASM_NODE_SHIM" <<'SHIM'
// Minimal WASI preview1 host, mirroring CPPBox's WasiCtxBuilder: stdin piped,
// the job directory preopened as ".". Exits 125 on a wasm trap so the caller
// can tell a trap from a program's own non-zero exit.
import { WASI } from 'node:wasi';
import { readFile } from 'node:fs/promises';
const [wasmPath, dir] = process.argv.slice(2);
const wasi = new WASI({ version: 'preview1', args: ['main'], env: {},
  preopens: { '.': dir }, returnOnExit: true });
const module = await WebAssembly.compile(await readFile(wasmPath));
try {
  const instance = await WebAssembly.instantiate(module, wasi.getImportObject());
  process.exitCode = wasi.start(instance);
} catch (err) {
  console.error(String((err && err.stack) || err));
  process.exitCode = 125;
}
SHIM
    return 0
  fi
  if command -v wasmtime >/dev/null 2>&1; then
    WASMTIME_BIN="$(command -v wasmtime)"
    WASM_RUN_KIND="wasmtime"
  fi
}

# Echoes the reason this module cannot run on wasm, or "" if it can.
wasm_skip_reason() { # wasm_skip_reason <module>
  if _matches_markers "$1" "$WASM_THREAD_MARKERS"; then
    echo "uses threads; CPPBox routes these to podman"
  elif _matches_markers "$1" "$WASM_UNSUPPORTED_MARKERS"; then
    echo "uses <execution>; wasi-sdk libc++ has no parallel algorithms"
  else
    echo ""
  fi
}

_matches_markers() { # _matches_markers <module> <regex>
  local depth
  depth="$(find_depth_for_module "$1")"
  # shellcheck disable=SC2086  # depth is an intentional find flag pair
  find "$REPO_ROOT/$1" $depth \( -name '*.cpp' -o -name '*.h' \) -print0 2>/dev/null |
    xargs -0 -r grep -l -E "$2" 2>/dev/null | head -1 | grep -q .
}

# --- Argument parsing --------------------------------------------------------

while (($# > 0)); do
  case "$1" in
    --build-only) BUILD_ONLY=1 ;;
    --verbose | -v) VERBOSE=1 ;;
    --keep | -k) KEEP=1 ;;
    --wasm) TARGET=wasm ;;
    --timeout)
      RUN_TIMEOUT="${2:?--timeout needs a value in seconds}"
      [[ "$RUN_TIMEOUT" =~ ^[0-9]+$ ]] || {
        echo "smoke_test.sh: --timeout expects whole seconds, got '$RUN_TIMEOUT'" >&2
        exit 2
      }
      shift
      ;;
    --help | -h) usage; exit 0 ;;
    -*) echo "smoke_test.sh: unknown option '$1'" >&2; usage >&2; exit 2 ;;
    *) FILTERS+=("$1") ;;
  esac
  shift
done

# --- Terminal styling (plain text when not a tty, e.g. piped into a file) ----

if [[ -t 1 ]]; then
  readonly C_RESET=$'\033[0m' C_RED=$'\033[31m' C_GREEN=$'\033[32m'
  readonly C_YELLOW=$'\033[33m' C_BLUE=$'\033[34m' C_DIM=$'\033[2m'
else
  readonly C_RESET='' C_RED='' C_GREEN='' C_YELLOW='' C_BLUE='' C_DIM=''
fi

# --- Portability -------------------------------------------------------------

TIMEOUT_CMD=""
resolve_timeout_cmd() {
  local candidate
  for candidate in timeout gtimeout; do
    if command -v "$candidate" >/dev/null 2>&1; then
      TIMEOUT_CMD="$candidate"
      return 0
    fi
  done
  printf '%swarning: no timeout/gtimeout on PATH; programs run without a time limit%s\n' \
    "$C_YELLOW" "$C_RESET" >&2
}

# Milliseconds since the epoch. Prefers bash 5's EPOCHREALTIME (no subprocess,
# no GNU dependency) and degrades to whole seconds where neither is available.
now_ms() {
  if [[ -n "${EPOCHREALTIME:-}" ]]; then
    local microseconds="${EPOCHREALTIME/[.,]/}"
    echo $((microseconds / 1000))
    return
  fi
  local nanoseconds
  nanoseconds="$(date +%s%N 2>/dev/null)"
  if [[ "$nanoseconds" =~ ^[0-9]+$ ]]; then
    echo $((nanoseconds / 1000000))
  else
    echo $(($(date +%s) * 1000)) # BSD date: second granularity only
  fi
}

# --- Helpers -----------------------------------------------------------------

is_expected_build_failure() {
  local module="$1" expected
  ((${#EXPECTED_BUILD_FAILURES[@]} == 0)) && return 1
  for expected in "${EXPECTED_BUILD_FAILURES[@]}"; do
    [[ "$module" == "$expected" ]] && return 0
  done
  return 1
}

matches_filter() {
  ((${#FILTERS[@]} == 0)) && return 0
  local module="$1" filter
  for filter in "${FILTERS[@]}"; do
    [[ "$module" == *"$filter"* ]] && return 0
  done
  return 1
}

json_escape() {
  local text="$1"
  text="${text//\\/\\\\}"
  text="${text//\"/\\\"}"
  text="${text//$'\n'/ }"
  text="${text//$'\t'/ }"
  printf '%s' "$text"
}

# A module is any top-level directory holding at least one .cpp file, excluding
# vendored code. Directories with no sources are listed separately so stale
# leftovers stay visible instead of silently vanishing from the report.
# Both emitters print to stdout so the caller can capture them; nothing is
# assigned to a global here, because process substitution would run them in a
# subshell and discard it.
discover_modules() {
  local dir
  for dir in "$REPO_ROOT"/*/; do
    dir="$(basename "${dir%/}")"
    [[ " $NON_MODULE_DIRS " == *" $dir "* ]] && continue
    compgen -G "$REPO_ROOT/$dir/*.cpp" >/dev/null && echo "$dir"
  done
}

discover_source_less_dirs() {
  local dir
  for dir in "$REPO_ROOT"/*/; do
    dir="$(basename "${dir%/}")"
    [[ " $NON_MODULE_DIRS " == *" $dir "* ]] && continue
    compgen -G "$REPO_ROOT/$dir/*.cpp" >/dev/null || echo "$dir"
  done
}

# --- Per-module phases -------------------------------------------------------

BUILD_WARNINGS=0
SOURCE_COUNT=0

build_module() { # build_module <module> <binary> <log>
  local module="$1" binary="$2" log="$3"
  local depth flags sources=()
  depth="$(find_depth_for_module "$module")"
  flags="$(flags_for_module "$module")"

  # shellcheck disable=SC2086  # depth is an intentional find flag pair
  while IFS= read -r source_file; do sources+=("$source_file"); done < <(
    find "$REPO_ROOT/$module" $depth -name '*.cpp' | sort
  )
  SOURCE_COUNT=${#sources[@]}

  if [[ "$TARGET" == wasm ]]; then
    local wasm_flags
    wasm_flags="$WASM_TARGET_FLAGS --std=$(std_for_module "$module") $(includes_for_module "$module")"
    { echo "# $WASM_CXX --sysroot=$WASM_SYSROOT $wasm_flags ${sources[*]} -o $binary"; } >"$log"
    # shellcheck disable=SC2086  # flag strings are intentionally word-split
    "$WASM_CXX" "--sysroot=$WASM_SYSROOT" $wasm_flags "${sources[@]}" -o "$binary" >>"$log" 2>&1
  else
    { echo "# $CXX $flags ${sources[*]} -o $binary"; } >"$log"
    # shellcheck disable=SC2086  # flags is intentionally word-split
    "$CXX" $flags "${sources[@]}" -o "$binary" >>"$log" 2>&1
  fi
  local status=$?
  BUILD_WARNINGS=$(grep -c 'warning:' "$log")
  return $status
}

# Runs the binary in a disposable copy of the module, so files it writes never
# touch the working tree. Returns the program's exit code, or 99 if the sandbox
# could not be prepared.
run_module() { # run_module <module> <binary> <log>
  local module="$1" binary="$2" log="$3"
  local sandbox="$SMOKE_DIR/run/$module"

  rm -rf "$sandbox"
  if ! mkdir -p "$sandbox" || ! cp -R "$REPO_ROOT/$module/." "$sandbox/" >"$log" 2>&1; then
    echo "harness: could not prepare sandbox $sandbox" >>"$log"
    return 99
  fi

  # 52-mc_gbm and 82-system write CSVs into output/ and do not create it.
  mkdir -p "$sandbox/output"

  # Modules disagree about their working directory: most open "./file" relative
  # to themselves, while 82-system opens "82-system/data/curve.csv" relative to
  # the repo root (its Makefile's `run` target cds to the root first). This
  # self-referential symlink makes both spellings resolve inside the sandbox.
  ln -sfn . "$sandbox/$module"

  local limit
  limit="$(timeout_for_module "$module")"

  # stdin is /dev/null so any module waiting on input hits EOF instead of
  # blocking until the timeout.
  #
  # For wasm the product is a module, not an executable: it runs under a WASI
  # host with the sandbox preopened as ".", mirroring CPPBox's
  # WasiCtxBuilder::preopened_dir(job_dir, ".").
  local -a command=()
  if [[ "$TARGET" == wasm ]]; then
    case "$WASM_RUN_KIND" in
      # -W exceptions=y is required: the modules are built with
      # -fwasm-exceptions, and the CLI (unlike CPPBox, which sets
      # Config::wasm_exceptions) leaves the proposal off by default, failing
      # with "exceptions proposal not enabled". Verified with wasmtime 46.0.3,
      # the version CPPBox pins.
      wasmtime) command=("$WASMTIME_BIN" run -W exceptions=y --dir "$sandbox::." "$binary") ;;
      node) command=(node --no-warnings "$WASM_NODE_SHIM" "$binary" "$sandbox") ;;
      *) echo "harness: no wasm runner available" >>"$log"; return 99 ;;
    esac
  else
    command=("$binary")
  fi

  (
    cd "$sandbox" || exit 99
    if [[ -n "$TIMEOUT_CMD" ]]; then
      "$TIMEOUT_CMD" -k 5 "$limit" "${command[@]}" </dev/null
    else
      "${command[@]}" </dev/null
    fi
  ) >"$log" 2>&1
}

# Outcome of the most recent module, reported through named globals rather than
# a packed string:
#   MODULE_STATUS      pass | build_fail | xfail | xpass | crash | timeout | exit | harness
#   MODULE_DETAIL      human-readable reason ("" when passing)
#   MODULE_DURATION_MS wall-clock milliseconds of the run phase (0 if not run)
#
# Exit-status policy: a fatal signal (128+N) is a crash; 124/137 is a timeout;
# any other non-zero exit is a failure, because no module here is meant to
# report an error status.
MODULE_STATUS=""
MODULE_DETAIL=""
MODULE_DURATION_MS=0

smoke_one_module() {
  local module="$1" binary="$2" build_log="$3" run_log="$4"
  MODULE_STATUS=""
  MODULE_DETAIL=""
  MODULE_DURATION_MS=0

  # Modules wasm cannot serve at all are skipped rather than failed.
  if [[ "$TARGET" == wasm ]]; then
    local skip_reason
    skip_reason="$(wasm_skip_reason "$module")"
    if [[ -n "$skip_reason" ]]; then
      MODULE_STATUS="skip_wasm"
      MODULE_DETAIL="$skip_reason"
      return
    fi
  fi

  local wasm_known=""
  [[ "$TARGET" == wasm ]] && wasm_known="$(wasm_expected_failure_reason "$module")"

  if ! build_module "$module" "$binary" "$build_log"; then
    if [[ -n "$wasm_known" ]]; then
      MODULE_STATUS="xfail_wasm"
      MODULE_DETAIL="$wasm_known"
      return
    fi
    if is_expected_build_failure "$module"; then
      MODULE_STATUS="xfail"
      MODULE_DETAIL="build fails by design (see AGENTS.md)"
    else
      MODULE_STATUS="build_fail"
      MODULE_DETAIL="compile or link failed"
    fi
    return
  fi

  if is_expected_build_failure "$module"; then
    MODULE_STATUS="xpass"
    MODULE_DETAIL="built cleanly, but AGENTS.md says it should not"
    return
  fi

  if ((BUILD_ONLY)); then
    MODULE_STATUS="pass"
    return
  fi

  local started_ms code
  started_ms="$(now_ms)"
  run_module "$module" "$binary" "$run_log"
  code=$?
  MODULE_DURATION_MS=$(($(now_ms) - started_ms))

  if ((code == 0)); then
    if [[ -n "$wasm_known" ]]; then
      MODULE_STATUS="xpass_wasm"
      MODULE_DETAIL="expected to fail on wasm but succeeded - update wasm_expected_failure_reason"
    else
      MODULE_STATUS="pass"
    fi
  elif [[ "$TARGET" == wasm ]] && ((code == 125)); then
    MODULE_STATUS="crash"
    MODULE_DETAIL="wasm trap (see run log)"
  elif ((code == 99)); then
    MODULE_STATUS="harness"
    MODULE_DETAIL="could not prepare or enter the run sandbox"
  elif ((code == 124 || code == 137)); then
    MODULE_STATUS="timeout"
    MODULE_DETAIL="no exit within $(timeout_for_module "$module")s"
  elif ((code > 128)); then
    local signal=$((code - 128))
    MODULE_STATUS="crash"
    MODULE_DETAIL="killed by signal ${signal} ($(kill -l "$signal" 2>/dev/null || echo unknown))"
  else
    MODULE_STATUS="exit"
    MODULE_DETAIL="non-zero exit status ${code}"
  fi

  if [[ -n "$wasm_known" && "$MODULE_STATUS" != pass && "$MODULE_STATUS" != xpass_wasm ]]; then
    MODULE_STATUS="xfail_wasm"
    MODULE_DETAIL="$wasm_known"
  fi
}

# --- Main --------------------------------------------------------------------

mkdir -p "$SMOKE_DIR/bin" "$SMOKE_DIR/log" "$SMOKE_DIR/run"
readonly RESULTS_JSONL="$SMOKE_DIR/results-$TARGET.jsonl"
: >"$RESULTS_JSONL"

resolve_timeout_cmd

if [[ "$TARGET" == wasm ]]; then
  resolve_wasm_toolchain || exit 2
  resolve_wasm_runner
fi

printf '%sFN6806 smoke test%s %s(target: %s)%s\n' "$C_BLUE" "$C_RESET" "$C_DIM" "$TARGET" "$C_RESET"
if [[ "$TARGET" == wasm ]]; then
  printf '%s  compiler : %s%s\n' "$C_DIM" "$("$WASM_CXX" --version 2>/dev/null | head -1)" "$C_RESET"
  printf '%s  sysroot  : %s%s\n' "$C_DIM" "$WASM_SYSROOT" "$C_RESET"
  printf '%s  runner   : %s%s\n' "$C_DIM" "${WASM_RUN_KIND:-none (build only)}" "$C_RESET"
else
  printf '%s  compiler : %s%s\n' "$C_DIM" "$("$CXX" --version 2>/dev/null | head -1)" "$C_RESET"
  printf '%s  flags    : %s%s\n' "$C_DIM" "$CXXFLAGS" "$C_RESET"
fi
printf '%s  scratch  : %s%s\n\n' "$C_DIM" "$SMOKE_DIR" "$C_RESET"

declare -a failures=()
pass_count=0 xfail_count=0 skip_count=0 warning_total=0 wasm_skip_count=0

mapfile -t MODULES < <(discover_modules)
mapfile -t NO_SOURCE_DIRS < <(discover_source_less_dirs)

for module in "${MODULES[@]}"; do
  matches_filter "$module" || { ((++skip_count)); continue; }

  binary="$SMOKE_DIR/bin/$module"
  [[ "$TARGET" == wasm ]] && binary="$binary.wasm"
  build_log="$SMOKE_DIR/log/$module.build.log"
  run_log="$SMOKE_DIR/log/$module.run.log"

  printf '%-32s ' "$module"
  smoke_one_module "$module" "$binary" "$build_log" "$run_log"
  warning_total=$((warning_total + BUILD_WARNINGS))

  warn_note=""
  ((BUILD_WARNINGS > 0)) && warn_note=", ${BUILD_WARNINGS} warn"

  case "$MODULE_STATUS" in
    pass)
      ((++pass_count))
      if ((BUILD_ONLY)); then
        printf '%sBUILD OK%s %s(%d src%s)%s\n' "$C_GREEN" "$C_RESET" \
          "$C_DIM" "$SOURCE_COUNT" "$warn_note" "$C_RESET"
      else
        printf '%sOK%s %s(%d ms, %d lines out%s)%s\n' "$C_GREEN" "$C_RESET" \
          "$C_DIM" "$MODULE_DURATION_MS" "$(wc -l <"$run_log")" "$warn_note" "$C_RESET"
        ((VERBOSE)) && sed 's/^/    /' "$run_log"
      fi
      ;;
    xfail)
      ((++xfail_count))
      printf '%sXFAIL%s %s%s%s\n' "$C_YELLOW" "$C_RESET" "$C_DIM" "$MODULE_DETAIL" "$C_RESET"
      ;;
    skip_wasm)
      ((++wasm_skip_count))
      printf '%sSKIP%s %s%s%s\n' "$C_BLUE" "$C_RESET" "$C_DIM" "$MODULE_DETAIL" "$C_RESET"
      ;;
    xfail_wasm)
      ((++xfail_count))
      printf '%sXFAIL-WASM%s %s%s%s\n' "$C_YELLOW" "$C_RESET" "$C_DIM" "$MODULE_DETAIL" "$C_RESET"
      ;;
    xpass_wasm)
      failures+=("$module: $MODULE_DETAIL")
      printf '%sXPASS-WASM%s %s%s%s\n' "$C_RED" "$C_RESET" "$C_DIM" "$MODULE_DETAIL" "$C_RESET"
      ;;
    build_fail)
      failures+=("$module: $MODULE_DETAIL")
      printf '%sBUILD FAIL%s %s%s%s\n' "$C_RED" "$C_RESET" "$C_DIM" "$build_log" "$C_RESET"
      grep -m3 -E 'error:' "$build_log" | sed 's/^/    /'
      ;;
    xpass)
      failures+=("$module: expected build failure, but it compiled")
      printf '%sXPASS%s %s%s%s\n' "$C_RED" "$C_RESET" "$C_DIM" "$MODULE_DETAIL" "$C_RESET"
      ;;
    *)
      failures+=("$module: $MODULE_DETAIL")
      printf '%s%s%s %s (%d ms) - %s%s%s\n' "$C_RED" "${MODULE_STATUS^^}" "$C_RESET" \
        "$MODULE_DETAIL" "$MODULE_DURATION_MS" "$C_DIM" "$run_log" "$C_RESET"
      tail -n5 "$run_log" | sed 's/^/    /'
      ;;
  esac

  printf '{"event":"module_result","target":"'"$TARGET"'","module":"%s","status":"%s","detail":"%s","duration_ms":%d,"sources":%d,"warnings":%d,"build_log":"%s","run_log":"%s"}\n' \
    "$(json_escape "$module")" "$MODULE_STATUS" "$(json_escape "$MODULE_DETAIL")" \
    "$MODULE_DURATION_MS" "$SOURCE_COUNT" "$BUILD_WARNINGS" \
    "$(json_escape "$build_log")" "$(json_escape "$run_log")" >>"$RESULTS_JSONL"
done

# --- Summary -----------------------------------------------------------------

printf '\n%s---%s\n' "$C_DIM" "$C_RESET"
printf 'passed %d   expected-fail %d   failed %d   warnings %d' \
  "$pass_count" "$xfail_count" "${#failures[@]}" "$warning_total"
((wasm_skip_count > 0)) && printf '   skipped-on-wasm %d' "$wasm_skip_count"
((skip_count > 0)) && printf '   filtered-out %d' "$skip_count"
printf '\n'

if ((${#NO_SOURCE_DIRS[@]} > 0)); then
  printf '%sno sources (skipped): %s%s\n' "$C_DIM" "${NO_SOURCE_DIRS[*]}" "$C_RESET"
fi

printf '{"event":"run_summary","target":"%s","passed":%d,"expected_fail":%d,"failed":%d,"filtered_out":%d,"warnings":%d,"skipped_on_wasm":%d}\n' \
  "$TARGET" "$pass_count" "$xfail_count" "${#failures[@]}" "$skip_count" "$warning_total" "$wasm_skip_count" >>"$RESULTS_JSONL"

printf '%sResults: %s%s\n' "$C_DIM" "$RESULTS_JSONL" "$C_RESET"

if ((${#failures[@]} > 0)); then
  printf '\n%sFailures:%s\n' "$C_RED" "$C_RESET"
  printf '  %s\n' "${failures[@]}"
  printf 'Logs: %s/log\n' "$SMOKE_DIR"
  exit 1
fi

((KEEP)) || rm -rf "$SMOKE_DIR/run"
printf '%sAll modules compile and run without crashing (target: %s).%s\n' "$C_GREEN" "$TARGET" "$C_RESET"
