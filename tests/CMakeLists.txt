# Tests for refinery library

add_executable(test_refine test_refine.cpp)
target_link_libraries(test_refine PRIVATE refinery::refinery GTest::gtest_main)
target_compile_options(test_refine PRIVATE -Wall -Wextra -Werror)

# Discover tests automatically
include(GoogleTest)
gtest_discover_tests(test_refine
    PROPERTIES TIMEOUT 60
)
