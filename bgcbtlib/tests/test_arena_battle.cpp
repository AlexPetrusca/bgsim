#include <random>
#include <gtest/gtest.h>

#include "Arena.h"
#include "Board.h"
#include "card/CardDb.h"
#include "card/Minion.h"

TEST(ArenaTest, SimpleOneSided) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Board boardB = Board({minionB1});

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 1);
}

TEST(ArenaTest, SimpleFiftyFifty) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Minion minionB2 = Minion("B2", 1, 1, 1);
    Board boardB = Board({minionB1, minionB2});

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 1);
}

TEST(ArenaTest, Deathrattle) {
    CardDb db;
    Minion minionA1 = db.get_minion(104551); // Harmless Bonehead
    Minion minionA2 = db.get_minion(104551); // Harmless Bonehead
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = db.get_minion(96778); // Houndmaster
    Board boardB = Board({minionB1});

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 3);
}

TEST(ArenaTest, DeathrattleOverflow) {
    CardDb db;
    Board boardA = Board();
    for (int i = 0; i < 7; i++) {
        Minion minion = db.get_minion(104551); // Harmless Bonehead
        minion.set_name("A" + std::to_string(i));
        boardA.add_minion(minion);
    }

    Board boardB = Board();
    for (int i = 0; i < 7; i++) {
        Minion minion = db.get_minion(104551); // Harmless Bonehead
        minion.set_name("B" + std::to_string(i));
        boardB.add_minion(minion);
    }

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 1);
}
