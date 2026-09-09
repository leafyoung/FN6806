# third_party

Vendored header-only third-party libraries, so modules referencing them
(`52-mc_gbm`'s optional Eigen path, `96-test-quantlib-xtensor-eigen`) don't
depend on a system package install being present - needed in particular
for cross-compiling to targets (e.g. wasm32) that don't share the host's
`/usr/include`.

## eigen3

[Eigen](https://eigen.tuxfamily.org/) 3.4.1, from
https://gitlab.com/libeigen/eigen/-/tree/3.4.1 (MPL2, see
`eigen3/COPYING.MPL2`). Only the header-only `Eigen/` directory is vendored
(no build step - Eigen is header-only). Pinned to the 3.4.x line to match
`#include <eigen3/Eigen/Dense>` and the Eigen 3.x API the existing modules
were written against; Eigen 5.x changed enough that it wasn't a safe drop-in
without re-checking every call site.

Compile with `-I third_party` (relative to the repo root) so
`#include <eigen3/Eigen/Dense>` resolves to `third_party/eigen3/Eigen/Dense`
- matching the same relative layout a system install at `/usr/include`
would have, just rooted here instead.
