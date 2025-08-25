#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "../include/card/Minion.h"

TEST(MinionTest, Print) {
    Minion minion = Minion("Alleycat", 1, 1, 1);
    std::string minion_str = minion.to_string();
    EXPECT_THAT(minion_str, testing::HasSubstr("Alleycat"));
    EXPECT_THAT(minion_str, testing::HasSubstr("1/1"));
}
