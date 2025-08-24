add_executable(bgcomlib_tests test_board.cpp
        test_minion.cpp)

target_link_libraries(bgcomlib_tests
        PRIVATE
        bgcomlib
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(bgcomlib_tests)