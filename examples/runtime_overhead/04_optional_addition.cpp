// 04_optional_addition.cpp — Proves integer Positive + Positive (returning
// std::optional) matches hand-written try_refine equivalent
//
// For integers, Positive is not preserved by addition (overflow), so
// operator+ returns std::optional<Refined<int, Positive>> via try_refine.
// The plain version calls try_refine directly with the raw sum.
//
// Expected: refined_add_positive and plain_add_positive produce identical
// assembly.

#include <refinery/refinery.hpp>

using namespace refinery;

using PosInt = Refined<int, Positive>;

__attribute__((noinline)) std::optional<PosInt>
refined_add_positive(PosInt a, PosInt b) {
    return a + b;
}

__attribute__((noinline)) std::optional<PosInt>
plain_add_positive(int a, int b) {
    return try_refine<Positive>(a + b);
}

int main() {
    auto a = PosInt(10, assume_valid);
    auto b = PosInt(20, assume_valid);

    volatile bool sink;
    sink = refined_add_positive(a, b).has_value();
    sink = plain_add_positive(10, 20).has_value();
    return 0;
}
