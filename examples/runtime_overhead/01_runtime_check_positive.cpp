// 01_runtime_check_positive.cpp — Proves runtime_check construction has no
// overhead vs hand-written check-and-throw
//
// Expected: refined_check_positive and plain_check_positive produce identical
// assembly (check value > 0, throw refinement_error if not, return value).

#include <rcpp/refined.hpp>

using namespace refined;

__attribute__((noinline)) int refined_check_positive(int value) {
    return Refined<int, Positive>(value, runtime_check).get();
}

__attribute__((noinline)) int plain_check_positive(int value) {
    if (!Positive(value)) {
        throw refinement_error(value);
    }
    return value;
}

int main() {
    volatile int sink;
    sink = refined_check_positive(42);
    sink = plain_check_positive(42);
    return 0;
}
