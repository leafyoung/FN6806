# Pointer Memory

## Reading the output

```text
0x7fff54ebbffc 0x7fff54ebbff8 0x7fff54ebbff4   <- main's a, b, c
-1 -1
```

`a`, `b`, `c` land at consecutive addresses 4 bytes apart, **in decreasing order** (`&a > &b > &c`) — the compiler is packing them in declaration order but the stack grows downward on x86-64, so later-declared locals get lower addresses. `&b - &a` is pointer arithmetic: it's `(byte_offset)/sizeof(int) = -4/4 = -1`, not the raw byte difference. This layout is **not guaranteed by the standard** — it's just what this compiler/ABI (unoptimized `-O0`, typical GCC/Clang on Linux) happens to do.

```text
abc!
0x7fff54ebbfac 0x7fff54ebbfa8 0x7fff54ebbfa4
-1 -1
```

`abc()`'s frame is 0x50 (80 bytes) below `main`'s frame — that gap is the call overhead: `main`'s own remaining stack usage, the pushed return address, saved registers, `abc`'s prologue (default-argument bookkeeping, alignment padding), etc. Same relative layout within the frame (`-1 -1`) since `abc` also just declares 3 plain ints in order.

```text
def!
0x7fff54ebbfac 0x7fff54ebbfa8 0x7fff54ebbfa4
-1 -1
```

This standalone `def()` call is at **the exact same address** as the `abc()` call above. Both are called directly from `main` — same call depth, same stack pointer at entry, same local-variable layout (3 ints) — so the previously-popped stack memory gets reused byte-for-byte. This is classic "dead stack memory isn't zeroed or moved" behavior; reading it after the frame is popped is technically accessing memory outside the object's lifetime (harmless here since it's not read, just its address printed).

```text
abc!
0x7fff54ebbfac ...
-1 -1
def!
0x7fff54ebbf5c 0x7fff54ebbf58 0x7fff54ebbf54
-1 -1
```

Second `abc(true)` call reuses the same address again (still depth 1 from `main`). But the `def()` called *from inside* `abc` is now depth 2, so it sits a further 0x50 below `abc`'s frame (`0xfac - 0x50 = 0xf5c`) — same per-call overhead as the `main → abc` jump, since `def` has an identical prologue shape.

```text
1,-1,4
```

Unrelated: this is just the heap array `p`, populated by list-initialization and freed correctly with `delete[]`.

**Takeaway:**

the exercise is really demonstrating that (1) `ptr_b - ptr_a` divides by `sizeof(T)`, and (2) stack frame addresses are deterministic *for a given call graph* on a given compiler/build (no ASLR effect within one run, ints not relocated, no inlining at `-O0`), but none of this is something you should rely on — it's implementation-defined/UB territory (reading `&a` after `def`/`abc` return is fine since you never dereference it, but the layout itself has zero standard guarantee and will change completely under `-O2`, a different ABI, or with stack protectors enabled).