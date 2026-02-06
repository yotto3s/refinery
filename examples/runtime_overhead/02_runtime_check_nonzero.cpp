// 02_runtime_check_nonzero.cpp — Proves runtime_check with NonZero matches
// hand-written check for both int and double
//
// Expected: refined and plain pairs produce identical assembly.

#include <refinery/refinery.hpp>

using namespace refinery;

__attribute__((noinline)) int refined_check_nonzero(int value) {
    return Refined<int, NonZero>(value, runtime_check).get();
}

__attribute__((noinline)) int plain_check_nonzero(int value) {
    if (!NonZero(value)) {
        throw refinement_error(value);
    }
    return value;
}

__attribute__((noinline)) double refined_check_nonzero_double(double value) {
    return Refined<double, NonZero>(value, runtime_check).get();
}

__attribute__((noinline)) double plain_check_nonzero_double(double value) {
    if (!NonZero(value)) {
        throw refinement_error(value);
    }
    return value;
}

int main() {
    volatile int isink;
    isink = refined_check_nonzero(1);
    isink = plain_check_nonzero(1);

    volatile double dsink;
    dsink = refined_check_nonzero_double(1.0);
    dsink = plain_check_nonzero_double(1.0);
    return 0;
}
