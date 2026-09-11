# AGENTS.md

This file provides guidance to Claude Code (claude.ai/code) when working with
code in this repository. `CLAUDE.md` simply imports it via `@AGENTS.md`.

## Project Overview

FN6806 is a C++ example repository for the course "OOP II - C++ Programming".
Each numbered directory is a self-contained module demonstrating specific C++
concepts, from basic class design through templates, smart pointers,
polymorphism, CRTP, concurrency, and expression templates.

## Build Commands

There is no top-level build system - each module is compiled independently.
Navigate into a module directory, or pass its sources explicitly:

```bash
# Most modules
clang++ -Wall -Wextra -std=c++17 -pthread -o main *.cpp

# C++20 modules (49d-variant_visit_cxx20, 81-concept)
clang++ -Wall -Wextra -std=c++20 -pthread -o main *.cpp

# Vendored headers (96-test-xtensor-eigen)
clang++ -Wall -Wextra -std=c++17 -pthread -I third_party -o main *.cpp

./main
```

**Warnings are not errors here.** Unlike FN6805, this repo builds without
`-Werror`, because some modules demonstrate warning-worthy code on purpose -
`60-exception` shows a narrowing conversion and `96-test-xtensor-eigen` has a
deliberately missing return. The smoke test counts warnings but never fails on
them.

`-pthread` is safe to pass everywhere and is required by `52-mc_gbm`,
`54-thread`, `55-thread-atomic`, `56-thread-struct`, `71-multithread_mc_pi`,
`72-thread-mtx-cv`, and `73-thread-local-prng`.

## Smoke Testing All Modules

```bash
./smoke_test.sh                    # build and run every module
./smoke_test.sh 54-thread 82       # only modules matching 54-thread or 82
./smoke_test.sh --build-only       # compile only, skip the run phase
./smoke_test.sh --verbose          # echo each program's stdout/stderr
./smoke_test.sh --timeout 120      # default per-program run timeout, seconds
./smoke_test.sh --help             # all options
```

`smoke_test.sh` compiles **each module in isolation**, automating the per-module
loop the manual commands above describe. A module passes if it compiles and then
runs to completion without crashing - output is never checked for correctness.

A full run takes roughly 80 seconds, dominated by `92-et-vec-benchmark` (~63 s);
`--build-only` finishes in a few seconds.

Details worth knowing before changing the script:

- Binaries, build logs, and run sandboxes go to `$TMPDIR/fn6806-smoke`
  (override with `SMOKE_DIR`), so a run never dirties `git status`.
- Every module also produces a machine-readable record in
  `$SMOKE_DIR/results.jsonl` - one JSON object per module with `status`,
  `detail`, `duration_ms`, `warnings`, and log paths, plus a final `run_summary`
  line. Parse that rather than scraping the human-readable table.
- `status` is one of `pass`, `xfail`, `xpass`, `build_fail`, `crash`, `timeout`,
  `exit`, or `harness` (the last meaning the runner itself failed, not the
  program).
- Per-module differences live in three small `case` functions near the top of
  the script, not scattered through the logic:
  - `flags_for_module` - C++20 for `49d-variant_visit_cxx20` and `81-concept`,
    `-I third_party` for `96-test-xtensor-eigen`.
  - `find_depth_for_module` - sources are top-level only, except `82-system`,
    which is searched recursively to pick up `observer/risk_limit_checker.cpp`
    (matching the `SRCS` list in its own `82-system/Makefile`).
  - `timeout_for_module` - `92-et-vec-benchmark` gets 240 s because it runs two
    100-million-iteration loops with no optimisation flags.
- Each program runs inside a disposable **copy** of its module, and the sandbox
  gets an `output/` subdirectory because `52-mc_gbm` and `82-system` write CSVs
  there without creating it.
- Modules disagree about their working directory: most open `"./file"` relative
  to themselves, while `82-system` opens `"82-system/data/curve.csv"` relative to
  the repo root (its Makefile's `run` target cds to the root first). The sandbox
  contains a self-referential symlink so both spellings resolve.
- `stdin` is `/dev/null`, so any module waiting on input hits EOF instead of
  blocking until the timeout.
- The script degrades instead of breaking on non-GNU systems: it accepts
  `timeout` or `gtimeout` and warns if neither exists, times runs with bash's
  `EPOCHREALTIME` rather than `date +%N`, and uses `case` statements instead of
  `declare -A` so bash 3.2 (stock macOS) works.
- `EXPECTED_BUILD_FAILURES` is empty: no module here is intentionally
  uncompilable. The mechanism is kept so a future teaching example can be listed
  and asserted to keep failing, reported as `XFAIL` (and `XPASS` if it starts
  compiling).
- Directories with no `.cpp` files are listed under "no sources (skipped)"
  rather than silently ignored - currently `46-virtual_dtor` and
  `48-crtp_vs_virtual`, which hold only stale binaries and were superseded by
  `46-virtual` and `48-crtp_vs_virtual_vs_policy`.
- Exit status is 0 only when every module passes.

### Targeting wasm (CPPBox classroom)

```bash
./smoke_test.sh --wasm                # build + run every module on wasm32-wasip1
./smoke_test.sh --wasm --build-only   # compile only
./smoke_test.sh --wasm 82 96          # filters work the same
```

Students run this code in **CPPBox** (`~/devv/fin/classroom`), a teaching IDE
that compiles to `wasm32-wasip1` with a bundled wasi-sdk and runs the module
under an embedded wasmtime, with **Native** (host clang++) as the other explicitly selectable backend and no fallback between them. `--wasm`
reproduces that path, so a green host run plus a green wasm run means the
modules work in the environment students actually use.

- Compile flags are copied verbatim from `cppbox-core/src/wasi_exec.rs::compile`
  — `-fwasm-exceptions` with the two `-mllvm` EH flags, the memory limits,
  `-lunwind`, and `-lc-printscan-long-double`. Change them only to track that
  file. `-std` and the `third_party` include path come from `std_for_module` /
  `includes_for_module`, shared with the host build so the targets cannot drift.
- The toolchain is found automatically: newest `wasi-sdk-*` under
  `$CPPBOX_ROOT/wasi-toolchain` (`CPPBOX_ROOT` defaults to
  `~/devv/fin/classroom`). Override with `WASI_SDK=`.
- The runner is `wasmtime` if it is on `PATH`, otherwise `node` (which ships a
  WASI preview1 host) via a small generated shim; `WASM_RUNNER=` forces one. The
  sandbox is preopened as `.`, mirroring CPPBox's
  `WasiCtxBuilder::preopened_dir(job_dir, ".")`.
- **Threads are decided by the run, not by a grep.** wasm32-wasip1 cannot
  spawn threads, but header presence is the wrong signal for that: `<atomic>`,
  `<mutex>` and `<shared_mutex>` all work single-threaded, and `<thread>` itself
  is harmless unless something actually spawns (`60-exception` needs it for
  `std::this_thread::sleep_for`; `70-chrono` inherits it from vendored `tz.h`).
  So a module runs, and if it dies with an uncaught exception *and* mentions
  `<thread>`/`<future>`/`<condition_variable>`, it is reported as `SKIP spawns
  threads` rather than a failure. Six FN6806 modules land there - `52-mc_gbm`,
  `54-thread`, `55-thread-atomic`, `56-thread-struct`, `71-multithread_mc_pi`,
  `72-thread-mtx-cv` - and they are what CPPBox's **Native** backend is for.
  This mirrors `wasi_exec::thread_failure_hint` in CPPBox: an uncaught C++
  exception reaches the host as only "thrown Wasm exception", identical to a
  `bad_alloc`, so the sources have to corroborate before blaming threads.

- Results land in `$SMOKE_DIR/results-wasm.jsonl` (host runs write
  `results-host.jsonl`), and every record carries a `target` field.
- `WASM_UNSUPPORTED_MARKERS` adds `<execution>` on top of CPPBox's thread list:
  the parallel algorithms need threads and are absent from the wasm sysroot. No
  module here uses it today (FN6805's `52-stl` does). CPPBox's own
  `uses_threading` does not list `<execution>`, so such code reaches wasm there
  and fails to compile instead of being routed to podman — worth adding upstream.
- `WASM_STACK_SIZE` (default 8 MiB) is the one flag added beyond CPPBox's set:
  wasi-sdk defaults the wasm stack to 64 KiB, which an ordinary large local
  array overflows, trapping with "memory access out of bounds". CPPBox should
  pass the same flag.
- Threads do **not** work on wasm and that is not going to change: wasi-threads
  compiles and links (wasi-sdk 34 ships a `wasm32-wasip1-threads` sysroot, and
  the module gets the right ABI — a `wasi.thread-spawn` import and a
  `wasi_thread_start` export), but under wasmtime 46.0.3 — the version CPPBox
  pins — `std::thread` still fails with `thread constructor failed: Resource
  temporarily unavailable`, and wasmtime warns that `-Sthreads` becomes a hard
  error in 47.0.0. Bytecode Alliance RFC 47 (merged May 2026) removes
  wasi-threads outright, pointing to WASIp3 cooperative threads near term and
  the shared-everything-threads proposal long term. The nine skipped modules
  stay on podman.
- `wasm_expected_failure_reason` declares known wasm limitations, reported as
  `XFAIL-WASM`. It is **empty today**: every non-threaded module builds and runs
  on wasm. `47-poly_type` was in it until `K.h`'s `Ksub` allocation was reduced
  from `vector<int> x(1'000'000'000)` (4 GB — impossible in wasm32's 4 GiB
  address space, `std::bad_alloc`, while succeeding on Linux via overcommit) to
  `100'000'000` (400 MB, which fits).
- If a module in that list starts working, the run reports `XPASS-WASM` and
  fails, so the table cannot rot silently. That is how the `47-poly_type` entry
  was caught and removed.
- The wasm build uses `-O2` (CPPBox's setting) while the host build uses no
  optimisation flag, so wasm can be *faster*: `92-et-vec-benchmark` takes ~2.5 s
  on wasm versus ~63 s on the host.

### Cleaning build artifacts

Use `git clean -Xnd` to preview and `git clean -Xfd` to delete. It removes only
gitignored files (`*.o`, `main`, `main-debug`, `thread_main`, `output/`, ...) and
never touches `.git`.

Do **not** clean with `find -delete`. `-delete` implies `-depth`, which disables
`-prune`, so a guard like `find . -path ./.git -prune -o -name main -delete`
still descends into `.git` and deletes `refs/heads/main` - silently detaching the
branch. If you must use `find`, use `-exec rm -f {} +` instead of `-delete`.

## Module Structure

Each module follows a consistent pattern:

- `main.cpp` - entry point; calls test/demo functions defined in companion files
- `*.h` / `*.cpp` pairs - declarations and implementations used by `main.cpp`

Every module holds exactly **one** `main()`, and sources sit at the module's top
level - `82-system` is the sole exception, with an `observer/` subdirectory.
Multi-file modules (`40-inheritance_basic`, `47-poly_type`, `54-thread`,
`60-exception`, `72-thread-mtx-cv`, `82-system`) split demonstrations into named
files that `main.cpp` declares and calls.

## Module Index

| Directory | Topic |
| --- | --- |
| `10-class_rectangle`, `12-class_point`, `13-class-using` | Basic class design, `using` declarations |
| `20-cards`, `22-operator` | Operator overloading |
| `30-template`, `30a-template_specialize`, `30b-template-in-class`, `30c-extend-stl`, `30d-template-matching` | Function templates, specialization, extending the STL |
| `31-print_container_template`, `34-class_template_static`, `36-class_template_conversion`, `37-class-template`, `44-template_interpolator` | Class templates, static members, conversions, a templated interpolator |
| `38-raw_pointer`, `39a-unique_ptr`, `39b-shared_ptr`, `39c-weak_ptr` | Pointer ownership and smart pointers |
| `40-inheritance_basic`, `42-inherit_reuse`, `45-inheritance_for_reuse`, `46-virtual` | Inheritance, virtual functions, virtual destructors |
| `47-poly_type`, `49c-poly_variant_visit`, `49d-variant_visit_cxx20`, `93-poly-functon-pointer` | Polymorphism via inheritance, `std::variant`, function pointers |
| `48-crtp_vs_virtual_vs_policy`, `49-crtp_mixin`, `49a-crtp_neuron`, `49b-crtp_demo`, `98-template-policy` | CRTP, mixins, policy-based design |
| `50-valarray`, `52-mc_gbm` | `std::valarray`; Monte Carlo Geometric Brownian Motion |
| `54-thread`, `55-thread-atomic`, `56-thread-struct`, `72-thread-mtx-cv`, `73-thread-local-prng` | `std::thread`, `std::atomic`, mutex/condition variable, thread-local PRNG |
| `60-exception` | Exception handling patterns |
| `70-chrono`, `71-multithread_mc_pi` | Timing; multi-threaded Monte Carlo pi |
| `80-hash`, `81-concept`, `82-system` | Hashing; C++20 concepts; a full pricing-engine application |
| `90-et-infinite-loop`, `91-et-base`, `92-et-vec-benchmark` | Expression templates: CRTP recursion pitfall, base design, benchmark |
| `95-pointer-memory`, `96-test-xtensor-eigen`, `97-test_variadic`, `99-naive_bayes` | Memory layout; xtensor/Eigen; variadic templates; naive Bayes |

`third_party/` holds vendored headers (Eigen 3, xtensor, xtl) and is not a
module - `smoke_test.sh` skips it.

## Working on a Module

Because modules are compiled independently, no commenting-out is ever needed:

```bash
./smoke_test.sh 54-thread --verbose    # build one module and see its output
```

`52-mc_gbm` writes CSVs to `output/`; create it first (`mkdir -p output`) when
running that module by hand.

## Source Header Links

Each module's `main.cpp` opens with a link to that directory on GitHub:

```cpp
// https://github.com/leafyoung/FN6806/tree/main/49-crtp_mixin
```

These replaced the original `replit.com/@YeKunlun/...` links. The Replit slugs
did not reliably track the directory name - `49-crtp_mixin` was
`49-crtpextenddelta`, `46-virtual` was `46-virtualdtor`, and
`48-crtp_vs_virtual_vs_policy` was `48-crtpvsvirtual` - so when adding a link,
derive it from the **directory name**, never from an old slug.

## C++ Standards Used

C++11 through C++20 depending on the module; most target C++17.

- Structured bindings, `constexpr`, `std::optional`, `std::variant` (C++17)
- `std::string_view`, fold expressions, `if constexpr` (C++17)
- Concepts and `requires` clauses (C++20, `81-concept`)
- `std::visit` with overload sets (`49c-poly_variant_visit`, `49d-variant_visit_cxx20`)
- `<thread>`, `<mutex>`, `<atomic>`, `<future>`, `<condition_variable>`
- `<chrono>` for timing and benchmarking

## Adding a New Module

1. Create a numbered directory following the naming convention (hex-like prefix
   plus descriptive name).
2. Add `main.cpp` as the entry point, with the GitHub link for the directory as
   its first line.
3. Split sub-topics into `*_topic.cpp` / `*_topic.h` pairs if the module covers
   several.
4. Keep includes relative; no install-time paths.
5. Run `./smoke_test.sh <new-dir>` - it is picked up automatically, with no
   registration step, as long as it holds at least one `.cpp`.
