// 05_optional_subtraction.cpp — Proves integer Positive - Positive (returning
// std::optional) matches hand-written try_refine equivalent
//
// Subtraction never preserves Positive, so operator- returns
// std::optional<Refined<int, Positive>> via try_refine.
// The plain version calls try_refine directly with the raw difference.
//
// Expected: refined_sub_positive and plain_sub_positive produce identical
// assembly.

#include <refinery/refinery.hpp>

using namespace refinery;

using PosInt = Refined<int, Positive>;

__attribute__((noinline)) std::optional<PosInt>
refined_sub_positive(PosInt a, PosInt b) {
    return a - b;
}

__attribute__((noinline)) std::optional<PosInt>
plain_sub_positive(int a, int b) {
    return try_refine<Positive>(a - b);
}

int main() {
    auto a = PosInt(30, assume_valid);
    auto b = PosInt(10, assume_valid);

    volatile bool sink;
    sink = refined_sub_positive(a, b).has_value();
    sink = plain_sub_positive(30, 10).has_value();
    return 0;
}
