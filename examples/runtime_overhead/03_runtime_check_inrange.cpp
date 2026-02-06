// 03_runtime_check_inrange.cpp — Proves runtime_check with InRange(1, 100)
// matches hand-written bounds check
//
// Expected: refined_check_inrange and plain_check_inrange produce identical
// assembly.

#include <refinery/refinery.hpp>

using namespace refinery;

static constexpr auto InRange1_100 = InRange(1, 100);

__attribute__((noinline)) int refined_check_inrange(int value) {
    return Refined<int, InRange1_100>(value, runtime_check).get();
}

__attribute__((noinline)) int plain_check_inrange(int value) {
    if (!InRange1_100(value)) {
        throw refinement_error(value);
    }
    return value;
}

int main() {
    volatile int sink;
    sink = refined_check_inrange(50);
    sink = plain_check_inrange(50);
    return 0;
}
