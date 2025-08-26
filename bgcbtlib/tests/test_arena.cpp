#include <random>
#include <gtest/gtest.h>

#include "AnalysisReport.h"
#include "Arena.h"
#include "Board.h"
#include "../../bgcomlib/include/card/Minion.h"

#define EXPECT_APPROX_EQ(val1, val2, error) \
    EXPECT_LE(std::abs(1.0 - ((double)(val1)) / ((double)(val2))), (error))

TEST(ArenaTest, OneSidedBattle) {
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

TEST(ArenaTest, FiftyFiftyBattle) {
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

TEST(ArenaTest, OneSidedAnalysis) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Board boardB = Board({minionB1});

    Arena arena = Arena(boardA, boardB);

    auto start = std::chrono::high_resolution_clock::now();
    int iterations = 100000;
    AnalysisReport report = arena.analyze(iterations);
    auto end = std::chrono::high_resolution_clock::now();
    auto raw_duration = duration_cast<std::chrono::microseconds>(end - start).count();
    auto milliseconds = static_cast<double>(raw_duration) * 0.001;
    std::cout << "Time taken: " << milliseconds << " milliseconds\n";

    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), iterations);
    EXPECT_EQ(report.ties(), 0);
    EXPECT_EQ(report.losses(), 0);

    EXPECT_EQ(report.in_dmg_median(), 0);
    EXPECT_EQ(report.in_dmg_quart1(), 0);
    EXPECT_EQ(report.in_dmg_quart3(), 0);

    EXPECT_EQ(report.out_dmg_median(), 1);
    EXPECT_EQ(report.out_dmg_quart1(), 1);
    EXPECT_EQ(report.out_dmg_quart3(), 1);
}

TEST(ArenaTest, FiftyFiftyAnalysis) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Minion minionB2 = Minion("B2", 1, 1, 1);
    Board boardB = Board({minionB1, minionB2});

    Arena arena = Arena(boardA, boardB);

    auto start = std::chrono::high_resolution_clock::now();
    int iterations = 100000;
    AnalysisReport report = arena.analyze(iterations);
    auto end = std::chrono::high_resolution_clock::now();
    auto raw_duration = duration_cast<std::chrono::microseconds>(end - start).count();
    auto milliseconds = static_cast<double>(raw_duration) * 0.001;
    std::cout << "Time taken: " << milliseconds << " milliseconds\n";

    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), 0);
    EXPECT_APPROX_EQ(report.ties(), iterations / 2, 0.05);
    EXPECT_APPROX_EQ(report.losses(), iterations / 2, 0.05);

    EXPECT_EQ(report.in_dmg_median(), 1);
    EXPECT_EQ(report.in_dmg_quart1(), 1);
    EXPECT_EQ(report.in_dmg_quart3(), 1);

    EXPECT_EQ(report.out_dmg_median(), 0);
    EXPECT_EQ(report.out_dmg_quart1(), 0);
    EXPECT_EQ(report.out_dmg_quart3(), 0);
}