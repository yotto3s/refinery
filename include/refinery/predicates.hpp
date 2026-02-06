// predicates.hpp - Standard predicates for refinement types
// Part of the C++26 Refinement Types Library

#ifndef REFINERY_PREDICATES_HPP
#define REFINERY_PREDICATES_HPP

#include <cmath>
#include <concepts>
#include <cstddef>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>

namespace refinery {

// --- Basic numeric predicates ---

// True if value > 0
inline constexpr auto Positive = [](auto v) constexpr {
    return v > decltype(v){0};
};

// True if value < 0
inline constexpr auto Negative = [](auto v) constexpr {
    return v < decltype(v){0};
};

// True if value >= 0
inline constexpr auto NonNegative = [](auto v) constexpr {
    return v >= decltype(v){0};
};

// True if value <= 0
inline constexpr auto NonPositive = [](auto v) constexpr {
    return v <= decltype(v){0};
};

// True if value != 0
inline constexpr auto NonZero = [](auto v) constexpr {
    return v != decltype(v){0};
};

// True if value == 0
inline constexpr auto Zero = [](auto v) constexpr {
    return v == decltype(v){0};
};

// --- Range predicates (curried) ---

// True if value > bound
inline constexpr auto GreaterThan = [](auto bound) constexpr {
    return [=](auto v) constexpr { return v > bound; };
};

// True if value >= bound
inline constexpr auto GreaterOrEqual = [](auto bound) constexpr {
    return [=](auto v) constexpr { return v >= bound; };
};

// True if value < bound
inline constexpr auto LessThan = [](auto bound) constexpr {
    return [=](auto v) constexpr { return v < bound; };
};

// True if value <= bound
inline constexpr auto LessOrEqual = [](auto bound) constexpr {
    return [=](auto v) constexpr { return v <= bound; };
};

// True if value == bound
inline constexpr auto EqualTo = [](auto bound) constexpr {
    return [=](auto v) constexpr { return v == bound; };
};

// True if value != bound
inline constexpr auto NotEqualTo = [](auto bound) constexpr {
    return [=](auto v) constexpr { return v != bound; };
};

// True if value is in closed interval [lo, hi]
inline constexpr auto InRange = [](auto lo, auto hi) constexpr {
    return [=](auto v) constexpr { return v >= lo && v <= hi; };
};

// True if value is in open interval (lo, hi)
inline constexpr auto InOpenRange = [](auto lo, auto hi) constexpr {
    return [=](auto v) constexpr { return v > lo && v < hi; };
};

// True if value is in half-open interval [lo, hi)
inline constexpr auto InHalfOpenRange = [](auto lo, auto hi) constexpr {
    return [=](auto v) constexpr { return v >= lo && v < hi; };
};

// --- Container/string predicates ---

// True if container is not empty (requires .empty() or .size())
inline constexpr auto NonEmpty = [](const auto& v) constexpr {
    if constexpr (requires { v.empty(); }) {
        return !v.empty();
    } else if constexpr (requires { v.size(); }) {
        return v.size() > 0;
    } else {
        static_assert(false, "Type does not support empty() or size()");
    }
};

// True if container is empty (requires .empty() or .size())
inline constexpr auto Empty = [](const auto& v) constexpr {
    if constexpr (requires { v.empty(); }) {
        return v.empty();
    } else if constexpr (requires { v.size(); }) {
        return v.size() == 0;
    } else {
        static_assert(false, "Type does not support empty() or size()");
    }
};

// True if container size >= min_size
inline constexpr auto SizeAtLeast = [](std::size_t min_size) constexpr {
    return [=](const auto& v) constexpr { return v.size() >= min_size; };
};

// True if container size <= max_size
inline constexpr auto SizeAtMost = [](std::size_t max_size) constexpr {
    return [=](const auto& v) constexpr { return v.size() <= max_size; };
};

// True if container size == exact_size
inline constexpr auto SizeExactly = [](std::size_t exact_size) constexpr {
    return [=](const auto& v) constexpr { return v.size() == exact_size; };
};

// True if container size is in [min_size, max_size]
inline constexpr auto SizeInRange = [](std::size_t min_size,
                                       std::size_t max_size) constexpr {
    return [=](const auto& v) constexpr {
        return v.size() >= min_size && v.size() <= max_size;
    };
};

// --- Pointer predicates ---

// True if pointer is not null
inline constexpr auto NotNull = [](auto* p) constexpr { return p != nullptr; };

// True if pointer is null
inline constexpr auto IsNull = [](auto* p) constexpr { return p == nullptr; };

// --- Divisibility predicates ---

// True if value is divisible by divisor (value % divisor == 0)
inline constexpr auto DivisibleBy = [](auto divisor) constexpr {
    return [=](auto v) constexpr { return v % divisor == 0; };
};

// True if value is even
inline constexpr auto Even = [](auto v) constexpr { return v % 2 == 0; };

// True if value is odd
inline constexpr auto Odd = [](auto v) constexpr { return v % 2 != 0; };

// --- Bitwise predicates ---

// True if value is a power of two (positive integer with exactly one bit set)
inline constexpr auto PowerOfTwo = [](auto v) constexpr
    requires std::integral<decltype(v)>
{ return v > 0 && (v & (v - 1)) == 0; };

// --- Floating-point predicates ---

// True if value is finite (not NaN and not +/-infinity)
inline constexpr auto Finite = [](auto v) constexpr {
    using T = decltype(v);
    return v == v // not NaN
           && v != std::numeric_limits<T>::infinity() &&
           v != -std::numeric_limits<T>::infinity();
};

// True if value is in [-1, 1]
inline constexpr auto Normalized = [](auto v) constexpr {
    return v >= decltype(v){-1} && v <= decltype(v){1};
};

// True if value is not NaN (uses IEEE 754: NaN != NaN)
inline constexpr auto NotNaN = [](auto v) constexpr { return v == v; };

// True if value is NaN (uses IEEE 754: NaN != NaN)
inline constexpr auto IsNaN = [](auto v) constexpr { return v != v; };

// True if value is +infinity or -infinity
inline constexpr auto IsInf = [](auto v) constexpr {
    using T = decltype(v);
    return v == std::numeric_limits<T>::infinity() ||
           v == -std::numeric_limits<T>::infinity();
};

// True if value is normal (not zero, subnormal, infinity, or NaN)
inline constexpr auto IsNormal = [](auto v) constexpr {
    return std::isnormal(v);
};

// True if |value - target| <= epsilon
inline constexpr auto ApproxEqual = [](auto target, auto epsilon) constexpr {
    return [=](auto v) constexpr {
        auto diff = v - target;
        return (diff < 0 ? -diff : diff) <= epsilon;
    };
};

// --- Testing predicates ---

// Always true (useful for testing)
inline constexpr auto Always = [](auto) constexpr { return true; };

// Always false (useful for testing)
inline constexpr auto Never = [](auto) constexpr { return false; };

} // namespace refinery

#endif // REFINERY_PREDICATES_HPP
