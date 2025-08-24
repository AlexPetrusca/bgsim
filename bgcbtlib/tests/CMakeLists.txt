add_executable(bgcbtlib_tests test_arena.cpp)

target_link_libraries(bgcbtlib_tests
        PRIVATE
        bgcbtlib
        bgcomlib
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bgcbtlib_tests)