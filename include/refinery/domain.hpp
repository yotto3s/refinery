#ifndef REFINERY_DOMAIN_HPP
#define REFINERY_DOMAIN_HPP

#include "refinery.hpp"

namespace refinery {

// Percentage type (0-100)
inline constexpr auto IsPercentage = InRange(0, 100);
using Percentage = Refined<int, IsPercentage>;

// Probability type (0.0-1.0)
inline constexpr auto IsProbability = [](double v) constexpr {
    return v >= 0.0 && v <= 1.0;
};
using Probability = Refined<double, IsProbability>;

// Unit interval [0, 1]
inline constexpr auto IsUnit = [](auto v) constexpr {
    return v >= decltype(v){0} && v <= decltype(v){1};
};
using UnitFloat = Refined<float, IsUnit>;
using UnitDouble = Refined<double, IsUnit>;

// Byte value (0-255)
inline constexpr auto IsByte = InRange(0, 255);
using ByteValue = Refined<int, IsByte>;

// Port number (1-65535)
inline constexpr auto IsPort = InRange(1, 65535);
using PortNumber = Refined<int, IsPort>;

// Natural numbers (positive integers)
using Natural = PositiveI32;

// Whole numbers (non-negative integers)
using Whole = NonNegativeI32;

// Helper to create a named predicate with documentation
#define DEFINE_PREDICATE(Name, ...) inline constexpr auto Name = __VA_ARGS__

} // namespace refinery

#endif // REFINERY_DOMAIN_HPP
