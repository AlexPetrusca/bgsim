#include <random>
#include <algorithm>
#include <gtest/gtest.h>

#include "AnalysisReport.h"
#include "Arena.h"
#include "Board.h"
#include "../../bgcomlib/include/card/Minion.h"
#include "card/CardDb.h"

#define EXPECT_APPROX_EQ(val1, val2, error) \
    EXPECT_LE(std::abs(1.0 - ((double)(val1)) / ((double)(val2))), (error))

template<typename R>
R time_fn(std::function<R()> fn) {
    auto start = std::chrono::high_resolution_clock::now();
    R output = fn();
    auto end = std::chrono::high_resolution_clock::now();
    auto raw_duration = duration_cast<std::chrono::microseconds>(end - start).count();
    auto milliseconds = static_cast<double>(raw_duration) * 0.001;
    std::cout << "Time taken: " << milliseconds << " milliseconds\n";
    return output;
}

static const int ITERATIONS = 10000;
static const float CI = 0.05;

TEST(ArenaTest, AnalyzeSimpleOneSided) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Board boardB = Board({minionB1});

    Arena arena = Arena(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), ITERATIONS);
    EXPECT_EQ(report.ties(), 0);
    EXPECT_EQ(report.losses(), 0);

    EXPECT_EQ(report.in_dmg_quart1(), 0);
    EXPECT_EQ(report.in_dmg_median(), 0);
    EXPECT_EQ(report.in_dmg_quart3(), 0);

    EXPECT_EQ(report.out_dmg_quart1(), 1);
    EXPECT_EQ(report.out_dmg_median(), 1);
    EXPECT_EQ(report.out_dmg_quart3(), 1);
}

TEST(ArenaTest, AnalyzeSimpleFiftyFifty) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Minion minionB2 = Minion("B2", 1, 1, 1);
    Board boardB = Board({minionB1, minionB2});

    Arena arena = Arena(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), 0);
    EXPECT_APPROX_EQ(report.ties(), ITERATIONS / 2, CI);
    EXPECT_APPROX_EQ(report.losses(), ITERATIONS / 2, CI);

    EXPECT_EQ(report.in_dmg_quart1(), 1);
    EXPECT_EQ(report.in_dmg_median(), 1);
    EXPECT_EQ(report.in_dmg_quart3(), 1);

    EXPECT_EQ(report.out_dmg_quart1(), 0);
    EXPECT_EQ(report.out_dmg_median(), 0);
    EXPECT_EQ(report.out_dmg_quart3(), 0);
}

TEST(ArenaTest, AnalyzeDeathrattle) {
    CardDb db;
    Minion minionA1 = db.get_minion(104551); // Harmless Bonehead
    Minion minionA2 = db.get_minion(104551); // Harmless Bonehead
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = db.get_minion(96778); // Houndmaster
    Board boardB = Board({minionB1});

    Arena arena = Arena(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), ITERATIONS);
    EXPECT_EQ(report.ties(), 0);
    EXPECT_EQ(report.losses(), 0);

    EXPECT_EQ(report.in_dmg_quart1(), 0);
    EXPECT_EQ(report.in_dmg_median(), 0);
    EXPECT_EQ(report.in_dmg_quart3(), 0);

    EXPECT_EQ(report.out_dmg_quart1(), 1);
    EXPECT_EQ(report.out_dmg_median(), 1);
    EXPECT_EQ(report.out_dmg_quart3(), 3);
}

TEST(ArenaTest, AnalyzeDeathrattleOverflow) {
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

    Arena arena = Arena(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_APPROX_EQ(report.wins(), 0.35 * ITERATIONS, CI);
    EXPECT_APPROX_EQ(report.ties(), 0.3 * ITERATIONS, CI);
    EXPECT_APPROX_EQ(report.losses(), 0.35 * ITERATIONS, CI);

    EXPECT_EQ(report.in_dmg_quart1(), 1);
    EXPECT_EQ(report.in_dmg_quart3(), 2);

    EXPECT_EQ(report.out_dmg_quart1(), 1);
    EXPECT_EQ(report.out_dmg_quart3(), 2);
}