#include <random>
#include <gtest/gtest.h>

#include "Arena.h"
#include "Board.h"
#include "card/CardDb.h"
#include "card/Minion.h"

TEST(ArenaBattleTest, SimpleOneSided) {
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

TEST(ArenaBattleTest, SimpleFiftyFifty) {
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

TEST(ArenaBattleTest, LongBattle) {
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

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Deathrattle) {
    CardDb db;
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER
    });

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 3);
}

TEST(ArenaBattleTest, DeathrattleOverflow) {
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

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 1);
}

TEST(ArenaBattleTest, DivineShieldTaunt) {
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

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 2);
}

TEST(ArenaBattleTest, DivineShieldReborn) {
    CardDb db;
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::PSYCH_O_TRON
    });

    Board boardB = Board::from_ids({
        CardDb::Id::COLOSSUS_OF_THE_SUN,
        CardDb::Id::PSYCH_O_TRON
    });

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 6);
}

TEST(ArenaBattleTest, TauntDeathrattle) {
    Board boardA = Board::from_ids({
        CardDb::Id::SEWER_RAT,
        CardDb::Id::SEWER_RAT,
        CardDb::Id::SEWER_RAT,
        CardDb::Id::HOUNDMASTER
    });

    Board boardB = Board::from_ids({
        CardDb::Id::COLOSSUS_OF_THE_SUN,
    });

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, OnDamageSummon) {
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::IMP_GANG_BOSS,
    });

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 1);
}

TEST(ArenaBattleTest, OnDamageSummonTaunt) {
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER,
    });

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, WindfuryDivineShield) {
    Board boardA = Board::from_ids({
        CardDb::Id::CRACKLING_CYCLONE_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER,
    });

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, CleaveTaunt) {
    Board boardA = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, CleaveResolveActiveMinion1) {
    testing::internal::CaptureStdout();

    Board boardA = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::VOIDLORD_G,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    std::mt19937 rng(123456);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);
    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);

    std::string output = testing::internal::GetCapturedStdout();
    std::cout << output << std::endl;
    EXPECT_TRUE(output.contains("\"Houndmaster\" (3) -> \"Foe Reaper 4000\" (0)"));
}

TEST(ArenaBattleTest, CleaveResolveActiveMinion2) {
    Board boardA = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER,
        CardDb::Id::VOIDLORD,
        CardDb::Id::VOIDLORD,
    });

    std::mt19937 rng(123456);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);
    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 6);
}

TEST(ArenaBattleTest, CleaveOverflow) {
    testing::internal::CaptureStdout();

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

    std::mt19937 rng(12345);
    Arena arena = Arena(boardA, boardB, rng);
    BattleReport report = arena.battle(true);
    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 6);

    std::string output = testing::internal::GetCapturedStdout();
    std::cout << output << std::endl;
    EXPECT_FALSE(output.contains("Guard Bot"));
}
