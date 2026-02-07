# Simplify Predicates Using Not<>

## Summary

Redefine 7 predicates in `predicates.hpp` as `Not<>` compositions of their counterparts, making the negation relationship explicit in code and eliminating duplicated logic.

## Changes

### predicates.hpp

Add `#include "compose.hpp"` to get access to `Not<>`.

Redefine the following predicates:

| Predicate | New definition | Was |
|---|---|---|
| `NonNegative` | `Not<Negative>` | `v >= decltype(v){0}` |
| `NonPositive` | `Not<Positive>` | `v <= decltype(v){0}` |
| `NonZero` | `Not<Zero>` | `v != decltype(v){0}` |
| `Odd` | `Not<Even>` | `v % 2 != 0` |
| `NonEmpty` | `Not<Empty>` | duplicated `if constexpr` dispatch |
| `NotNull` | `Not<IsNull>` | `p != nullptr` |
| `NotNaN` | `Not<IsNaN>` | `v == v` |

Reorder definitions so primary predicates appear before their `Not<>` counterparts in each section.

### Unchanged predicates

- **`NotEqualTo`** stays as-is. It's a curried factory (`NotEqualTo(bound)` returns a predicate). Can't express as `Not<EqualTo(bound)>` in a factory lambda because `bound` isn't a compile-time template argument inside the lambda body. Users can still write `Not<EqualTo(5)>` directly.
- **`Finite` / `IsInf`** are NOT negations. `Finite` rejects both NaN and infinity; `Not<IsInf>` would accept NaN. Both stay as independent definitions.

### Other files

No changes needed. Trait specializations in `operations.hpp` (`preserves<NonNegative, ...>`, `implies<Positive, NonZero>`, etc.) reference predicates by name. The name resolves to the same NTTP value everywhere regardless of how the predicate is defined internally.

## Testing

Build and run `ctest --test-dir build --output-on-failure` to verify all existing tests pass, including trait/implication tests and assembly comparisons.
