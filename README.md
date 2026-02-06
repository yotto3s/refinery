# rcpp - C++26 Refinement Types Library

A header-only library providing Liquid Haskell-style refinement types for C++26, using GCC reflection for rich compile-time error messages.

## Features

- **Compile-time verification**: Values are verified at compile time using `consteval` constructors
- **Runtime verification**: Optional runtime checking with `runtime_check` tag
- **Reflection-powered diagnostics**: Compile-time error messages include the actual violating value via C++26 reflection
- **Standard predicates**: `Positive`, `NonZero`, `NonNegative`, `InRange`, `Even`, `Odd`, etc.
- **Predicate composition**: `All<P1,P2>`, `Any<P1,P2>`, `Not<P>`, `If<P1,P2>`, etc.
- **Type-safe operations**: `safe_divide`, `abs`, `square`, `refined_min/max`

## Requirements

- GCC 16+ built from source with `-freflection` support (C++26 reflection)
- CMake 3.20+

## Building with a GCC build tree

If you built GCC from source and have an uninstalled build tree, use the provided toolchain file:

```bash
cmake -B build --toolchain cmake/xg++-toolchain.cmake \
      -DGCC_BUILD_DIR=/path/to/gcc/build/gcc
cmake --build build
ctest --test-dir build
```

`GCC_BUILD_DIR` should point to the `gcc/` subdirectory of your GCC build tree (the directory containing `xg++` and `cc1plus`). The toolchain file automatically handles:
- Setting `xg++` as the compiler with the `-B` flag for `cc1plus` lookup
- Adding `-nostdinc++` and the correct `-isystem` paths for the build-tree libstdc++
- Linker paths and rpath for the build-tree libstdc++

## Usage

```cpp
#include <rcpp/refine.hpp>
using namespace refine;

// Compile-time verified
constexpr PositiveInt x{42};      // OK
// PositiveInt y{-1};             // Compile error: "Refinement violation: -1 does not satisfy predicate"

// Runtime checked
auto z = try_refine<PositiveInt>(user_input);
if (z) use(*z);

// Type-safe division
template<typename T>
T safe_divide(T num, Refined<T, NonZero> denom) {
    return num / denom.get();  // Can never divide by zero!
}
```

## Building

With an installed GCC 16+:

```bash
cmake -B build -DCMAKE_CXX_COMPILER=g++-16
cmake --build build
ctest --test-dir build
```

With an uninstalled GCC build tree (see above):

```bash
cmake -B build --toolchain cmake/xg++-toolchain.cmake \
      -DGCC_BUILD_DIR=/path/to/gcc/build/gcc
cmake --build build
ctest --test-dir build
```

## Installation

```bash
cmake --install . --prefix /usr/local
```

Then in your project:

```cmake
find_package(rcpp REQUIRED)
target_link_libraries(your_target PRIVATE rcpp::rcpp)
```

## License

MIT License
