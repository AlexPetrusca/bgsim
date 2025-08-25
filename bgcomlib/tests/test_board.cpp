#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "Board.h"
#include "../include/card/Minion.h"

TEST(BoardTest, Print) {
    Minion minion1 = Minion("Alleycat", 1, 1, 1);
    Minion minion2 = Minion("Murloc Tidehunter", 1, 2, 1);
    Board board = Board({minion1, minion2});
    std::string board_str = board.to_string();
    EXPECT_THAT(board_str, testing::HasSubstr("Alleycat"));
    EXPECT_THAT(board_str, testing::HasSubstr("1/1"));
    EXPECT_THAT(board_str, testing::HasSubstr("Murloc Tidehunter"));
    EXPECT_THAT(board_str, testing::HasSubstr("2/1"));
}
