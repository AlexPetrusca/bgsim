#include <random>
#include <gtest/gtest.h>

#include "Arena.h"
#include "Board.h"
#include "Minion.h"

class ArenaTest : public ::testing::Test {
protected:
    inline static std::mt19937 rng;

    static void SetUpTestSuite() {
        rng.seed(12345);
    }
};

TEST_F(ArenaTest, SimpleFight) {
    Minion minionA1 = Minion("A1", 1, 1, 1);
    Minion minionA2 = Minion("A2", 1, 2, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Board boardB = Board({minionB1});

    Arena arena = Arena(boardA, boardB, rng);
    arena.battle();
}
