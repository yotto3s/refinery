# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build & Test

```bash
# Configure with GCC build-tree xg++ (uninstalled)
cmake -B build --toolchain cmake/xg++-toolchain.cmake \
      -DGCC_BUILD_DIR=/path/to/gcc/build/gcc

# Or with an installed GCC 16+
cmake -B build -DCMAKE_CXX_COMPILER=g++-16

# Build and test
cmake --build build
ctest --test-dir build --output-on-failure

# Direct test execution
./build/tests/test_refine
```

Requires GCC 16+ with C++26 reflection support. The `-freflection` flag is added automatically by CMake for GCC. The `xg++-toolchain.cmake` file handles `-B`, `-nostdinc++`, libstdc++ include/link paths for uninstalled GCC build trees.

## Architecture

Header-only C++26 library providing Liquid Haskell-style refinement types. Everything lives in the `refine::` namespace.

All public headers live in `include/rcpp/`.

### Header Dependency Graph

```
refine.hpp  (main entry point, type aliases, convenience macros)
  ├── diagnostics.hpp   (refinement_error, tag types, reflection-based formatting)
  ├── predicates.hpp    (30+ standard predicates: Positive, NonZero, InRange, Even, etc.)
  ├── compose.hpp       (All<>, Any<>, Not<>, If<>, runtime composition)
  ├── refined.hpp       (core Refined<T, Predicate> template)
  └── operations.hpp    (safe arithmetic with predicate preservation traits)
```

### Core Design

**`Refined<T, Predicate>`** wraps a value of type `T` and guarantees `Predicate(value)` holds. Three construction modes:
- `consteval` (default): compile-time verified, uses `std::meta::exception` on failure
- `runtime_check` tag: throws `refinement_error` on failure
- `assume_valid` tag: unchecked, for trusted contexts

**Predicates** are constexpr callable objects (lambdas) passed as non-type template parameters. Curried predicates (e.g., `InRange(lo, hi)`, `GreaterThan(n)`) return new predicates.

**Operations** (`operations.hpp`) use a `traits::preserves<Predicate, Operation>` trait system to determine if arithmetic preserves a refinement (e.g., Positive + Positive = Positive). When preservation is provable, operations return `Refined`; otherwise they return `std::optional<Refined>`.

### Key Type Aliases (defined in `refine.hpp`)

`PositiveInt`, `NonZeroInt`, `NonNegativeInt`, `Percentage`, `Probability`, `ByteValue`, `PortNumber`, `Natural`, `Whole`, etc. Created via `DEFINE_REFINED_TYPE` and `DEFINE_PREDICATE` macros.
