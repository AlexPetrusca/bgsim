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

static const int ITERATIONS = 10000;
static const float CI = 0.05;

TEST(ArenaAnalysisTest, SimpleOneSided) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Board boardB = Board({minionB1});

    Arena arena = Arena::from_boards(boardA, boardB);
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

TEST(ArenaAnalysisTest, SimpleFiftyFifty) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Minion minionB2 = Minion("B2", 1, 1, 1);
    Board boardB = Board({minionB1, minionB2});

    Arena arena = Arena::from_boards(boardA, boardB);
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

TEST(ArenaAnalysisTest, LongBattle) {
    Board boardA;
    for (int i = 0; i < 7; i++) {
        Minion minion = Minion("A" + std::to_string(i), 1, 5, 150);
        boardA.summon_minion(minion);
    }

    Board boardB;
    for (int i = 0; i < 7; i++) {
        Minion minion = Minion("B" + std::to_string(i), 1, 5, 150);
        boardB.summon_minion(minion);
    }

    Arena arena = Arena::from_boards(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), 0);
    EXPECT_EQ(report.ties(), ITERATIONS);
    EXPECT_EQ(report.losses(), 0);

    EXPECT_EQ(report.in_dmg_quart1(), 0);
    EXPECT_EQ(report.in_dmg_median(), 0);
    EXPECT_EQ(report.in_dmg_quart3(), 0);

    EXPECT_EQ(report.out_dmg_quart1(), 0);
    EXPECT_EQ(report.out_dmg_median(), 0);
    EXPECT_EQ(report.out_dmg_quart3(), 0);
}

TEST(ArenaAnalysisTest, Deathrattle) {
    CardDb db;
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER
    });

    Arena arena = Arena::from_boards(boardA, boardB);
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

TEST(ArenaAnalysisTest, DeathrattleOverflow) {
    CardDb db;
    Board boardA;
    for (int i = 0; i < 7; i++) {
        Minion minion = db.get_minion(CardDb::Id::HARMLESS_BONEHEAD);
        minion.set_name("A" + std::to_string(i));
        boardA.summon_minion(minion);
    }

    Board boardB;
    for (int i = 0; i < 7; i++) {
        Minion minion = db.get_minion(CardDb::Id::HARMLESS_BONEHEAD);
        minion.set_name("B" + std::to_string(i));
        boardB.summon_minion(minion);
    }

    Arena arena = Arena::from_boards(boardA, boardB);
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

TEST(ArenaAnalysisTest, DivineShieldTaunt) {
    CardDb db;
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::PSYCH_O_TRON
    });

    Board boardB = Board::from_ids({
        CardDb::Id::PSYCH_O_TRON,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::PSYCH_O_TRON,
    });

    Arena arena = Arena::from_boards(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), ITERATIONS);
    EXPECT_EQ(report.ties(), 0);
    EXPECT_EQ(report.losses(), 0);

    EXPECT_EQ(report.in_dmg_quart1(), 0);
    EXPECT_EQ(report.in_dmg_median(), 0);
    EXPECT_EQ(report.in_dmg_quart3(), 0);

    EXPECT_EQ(report.out_dmg_quart1(), 1);
    EXPECT_EQ(report.out_dmg_median(), 2);
    EXPECT_EQ(report.out_dmg_quart3(), 2);
}

TEST(ArenaAnalysisTest, CleaveOverflow) {
    Board boardA = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::VOIDLORD,
        CardDb::Id::VOIDLORD,
        CardDb::Id::SECURITY_ROVER,
        CardDb::Id::VOIDLORD,
        CardDb::Id::SECURITY_ROVER,
        CardDb::Id::VOIDLORD,
        CardDb::Id::VOIDLORD,
    });

    Arena arena = Arena::from_boards(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_APPROX_EQ(report.wins(), 0.3 * ITERATIONS, CI);
    EXPECT_APPROX_EQ(report.ties(), 0.315 * ITERATIONS, CI);
    EXPECT_APPROX_EQ(report.losses(), 0.385 * ITERATIONS, CI);

    EXPECT_EQ(report.in_dmg_quart1(), 1);
    EXPECT_EQ(report.in_dmg_median(), 2);
    EXPECT_EQ(report.in_dmg_quart3(), 3);

    EXPECT_EQ(report.out_dmg_quart1(), 6);
    EXPECT_EQ(report.out_dmg_median(), 6);
    EXPECT_EQ(report.out_dmg_quart3(), 6);
}

TEST(ArenaAnalysisTest, CleaveResolveActiveMinion) {
    Board boardA = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER,
        CardDb::Id::VOIDLORD,
        CardDb::Id::VOIDLORD,
    });

    Arena arena = Arena::from_boards(boardA, boardB);
    AnalysisReport report = arena.analyze(ITERATIONS);
    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), ITERATIONS);
    EXPECT_EQ(report.ties(), 0);
    EXPECT_EQ(report.losses(), 0);

    EXPECT_EQ(report.in_dmg_quart1(), 0);
    EXPECT_EQ(report.in_dmg_median(), 0);
    EXPECT_EQ(report.in_dmg_quart3(), 0);

    EXPECT_EQ(report.out_dmg_quart1(), 6);
    EXPECT_EQ(report.out_dmg_median(), 6);
    EXPECT_EQ(report.out_dmg_quart3(), 6);
}

// todo: should we include damage from vulgar homunculus + rylok in damage report?
TEST(ArenaAnalysisTest, NoPlayerDamageOnAnalysis) {
    Board boardA = Board::from_ids({
        CardDb::Id::VULGAR_HOMUNCULUS_G,
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::VULGAR_HOMUNCULUS,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
    });

    Arena arena = Arena::from_boards(boardA, boardB);
    int health_before = arena.playerA().total_health();
    AnalysisReport report = arena.analyze(ITERATIONS);
    int health_after = arena.playerA().total_health();
    std::cout << report << std::endl;

    EXPECT_EQ(report.wins(), 0);
    EXPECT_APPROX_EQ(report.ties(), 0.055 * ITERATIONS, CI);
    EXPECT_APPROX_EQ(report.losses(), 0.955 * ITERATIONS, CI);

    EXPECT_EQ(report.in_dmg_quart1(), 1);
    EXPECT_EQ(report.in_dmg_median(), 3);
    EXPECT_EQ(report.in_dmg_quart3(), 4);

    EXPECT_EQ(report.out_dmg_quart1(), 0);
    EXPECT_EQ(report.out_dmg_median(), 0);
    EXPECT_EQ(report.out_dmg_quart3(), 0);

    EXPECT_EQ(health_before - health_after, 0);
}
