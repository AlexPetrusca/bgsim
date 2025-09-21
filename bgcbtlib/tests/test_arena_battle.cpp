#include <random>
#include <gtest/gtest.h>

#include "Arena.h"
#include "Board.h"
#include "card/CardDb.h"
#include "card/Minion.h"
#include "util/Random.h"

TEST(ArenaBattleTest, SimpleOneSided) {
    Minion minionA1 = Minion("A1", 1, 2, 1);
    Minion minionA2 = Minion("A2", 1, 1, 1);
    Board boardA = Board({minionA1, minionA2});

    Minion minionB1 = Minion("B1", 1, 2, 2);
    Board boardB = Board({minionB1});

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);
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

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);
    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 6);

    std::string output = testing::internal::GetCapturedStdout();
    std::cout << output << std::endl;
    EXPECT_FALSE(output.contains("Guard Bot"));
}

TEST(ArenaBattleTest, Poisonous) {
    Board boardA = Board::from_ids({
        CardDb::Id::TIDE_ORACLE_MORGL,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER,
        CardDb::Id::SECURITY_ROVER,
        CardDb::Id::FOE_REAPER_4000_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Venomous) {
    Board boardA = Board::from_ids({
        CardDb::Id::DEADLY_SPORE,
        CardDb::Id::DEADLY_SPORE,
        CardDb::Id::DEADLY_SPORE,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, VenomousLostOnAttack) {
    Minion deadlier_spore = Minion("Deadlier Spore", 1, 3, 10);
    deadlier_spore.set(Keyword::VENOMOUS);
    Board boardA = Board({
        deadlier_spore
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER_G
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, VenomousLostOnDefend) {
    Minion deadlier_spore = Minion("Deadlier Spore", 1, 3, 13);
    deadlier_spore.set(Keyword::VENOMOUS);
    Board boardA = Board({
        deadlier_spore
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER_G,
        CardDb::Id::ALLEYCAT,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, CleaveDoesntProcPoison) {
    Board boardA = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000_G
    });

    Board boardB = Board::from_ids({
        CardDb::Id::ALLEYCAT,
        CardDb::Id::TIDE_ORACLE_MORGL,
        CardDb::Id::GUARD_BOT_T,
        CardDb::Id::DEADLY_SPORE,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 6);
}

TEST(ArenaBattleTest, DivineShieldPoison) {
    Board boardA = Board::from_ids({
        CardDb::Id::COLOSSUS_OF_THE_SUN_G
    });

    Board boardB = Board::from_ids({
        CardDb::Id::DEADLY_SPORE,
        CardDb::Id::TIDE_ORACLE_MORGL,
        CardDb::Id::TIDE_ORACLE_MORGL,
        CardDb::Id::DEADLY_SPORE,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SelflessHero) {
    Board boardA = Board::from_ids({
        CardDb::Id::SELFLESS_HERO,
        CardDb::Id::ALLEYCAT
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SelflessHeroNoValidTarget) {
    Board boardA = Board::from_ids({
        CardDb::Id::SELFLESS_HERO,
        CardDb::Id::PSYCH_O_TRON
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::GUARD_BOT_T
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SpawnOfNzoth) {
    Board boardA = Board::from_ids({
        CardDb::Id::SPAWN_OF_NZOTH,
        CardDb::Id::ALLEYCAT,
        CardDb::Id::ALLEYCAT
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SEWER_RAT,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SpawnOfNzothGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::SPAWN_OF_NZOTH_G,
        CardDb::Id::ALLEYCAT,
        CardDb::Id::ALLEYCAT,
        CardDb::Id::ALLEYCAT,
        CardDb::Id::ALLEYCAT,
        CardDb::Id::ALLEYCAT,
        CardDb::Id::ALLEYCAT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SEWER_RAT_G,
        CardDb::Id::SEWER_RAT_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, FiendishServant) {
    Board boardA = Board::from_ids({
        CardDb::Id::FIENDISH_SERVANT,
        CardDb::Id::FIENDISH_SERVANT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::PSYCH_O_TRON
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ImpulsiveTrickster) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMPULSIVE_TRICKSTER,
        CardDb::Id::FIENDISH_SERVANT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::PSYCH_O_TRON
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ImpulsiveTricksterChain) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMPULSIVE_TRICKSTER,
        CardDb::Id::IMPULSIVE_TRICKSTER,
        CardDb::Id::IMPULSIVE_TRICKSTER,
        CardDb::Id::IMPULSIVE_TRICKSTER,
        CardDb::Id::IMPULSIVE_TRICKSTER,
        CardDb::Id::IMPULSIVE_TRICKSTER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000,
        CardDb::Id::FOE_REAPER_4000
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, GoldenImpulsiveTricksterChain) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMPULSIVE_TRICKSTER_G,
        CardDb::Id::IMPULSIVE_TRICKSTER_G,
        CardDb::Id::IMPULSIVE_TRICKSTER_G,
        CardDb::Id::IMPULSIVE_TRICKSTER_G,
        CardDb::Id::IMPULSIVE_TRICKSTER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000_G,
        CardDb::Id::FOE_REAPER_4000_G,
        CardDb::Id::FOE_REAPER_4000,
        CardDb::Id::FOE_REAPER_4000,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RylakMetalheadProced) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::ALLEYCAT,
        CardDb::Id::TABBYCAT_T,
        CardDb::Id::TABBYCAT_T,
        CardDb::Id::TABBYCAT_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RylakMetalheadNotProced) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::TABBYCAT_T,
        CardDb::Id::TABBYCAT_T,
        CardDb::Id::TABBYCAT_T,
        CardDb::Id::TABBYCAT_T,
        CardDb::Id::TABBYCAT_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, GoldenRylakMetalhead) {
    Board boardA = Board::from_ids({
        CardDb::Id::TABBYCAT_T_G,
        CardDb::Id::ALLEYCAT_G,
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::ALLEYCAT_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RaceSpecificEnchantProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::TABBYCAT_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RaceSpecificEnchantNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::PRIMALFIN_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 2);
}

TEST(ArenaBattleTest, RaceSpecificEnchantProcSameProps) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HALF_SHELL_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ScavengingHyenaProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::ALLEYCAT_G,
        CardDb::Id::SCAVENGING_HYENA,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ScavengingHyenaNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::PRIMALFIN_T_G,
        CardDb::Id::SCAVENGING_HYENA,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 3);
}

TEST(ArenaBattleTest, ScavengingHyenaMultiple) {
    Board boardA = Board::from_ids({
        CardDb::Id::ALLEYCAT_G,
        CardDb::Id::SCAVENGING_HYENA,
        CardDb::Id::SCAVENGING_HYENA,
        CardDb::Id::SCAVENGING_HYENA,
        CardDb::Id::SCAVENGING_HYENA,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, DireWolfAlphaLives) {
    Board boardA = Board::from_ids({
        CardDb::Id::ALLEYCAT,
        CardDb::Id::DIRE_WOLF_ALPHA,
        CardDb::Id::ALLEYCAT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, DireWolfAlphaDies) {
    Board boardA = Board::from_ids({
        CardDb::Id::ALLEYCAT,
        CardDb::Id::DIRE_WOLF_ALPHA,
        CardDb::Id::ALLEYCAT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 3);
}

TEST(ArenaBattleTest, DireWolfAlphaWithSummons) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMP_GANG_BOSS,
        CardDb::Id::DIRE_WOLF_ALPHA,
        CardDb::Id::SEWER_RAT,
        CardDb::Id::DIRE_WOLF_ALPHA,
        CardDb::Id::SEWER_RAT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MurlocWarleader) {
    Board boardA = Board::from_ids({
        CardDb::Id::PRIMALFIN_T_G,
        CardDb::Id::MURLOC_WARLEADER,
        CardDb::Id::MURLOC_WARLEADER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::SAVANNAH_HIGHMANE,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Imprisoner) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMPRISONER,
        CardDb::Id::IMPRISONER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Mecharoo) {
    Board boardA = Board::from_ids({
        CardDb::Id::MECHAROO,
        CardDb::Id::MECHAROO_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MicroMachine) {
    Board boardA = Board::from_ids({
        CardDb::Id::MICRO_MACHINE,
        CardDb::Id::MICRO_MACHINE_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MicroMachineProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::MICRO_MACHINE,
        CardDb::Id::MICRO_MACHINE_G,
    });
    boardA.turn_start();

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MurlocTidecallerNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::MURLOC_TIDECALLER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 3);
}

TEST(ArenaBattleTest, MurlocTidecallerProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
        CardDb::Id::MURLOC_TIDECALLER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MurlocTidehunter) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::MURLOC_TIDEHUNTER_G,
        CardDb::Id::MURLOC_SCOUT_T,
        CardDb::Id::MURLOC_SCOUT_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RighteousProtector) {
    Board boardA = Board::from_ids({
        CardDb::Id::RIGHTEOUS_PROTECTOR,
        CardDb::Id::RIGHTEOUS_PROTECTOR_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HYENA_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RockpoolHunterProcOnSelf) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::ROCKPOOL_HUNTER,
        CardDb::Id::JO_E_BOT_T_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 1);
}

TEST(ArenaBattleTest, RockpoolHunterProcOnOther) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::ROCKPOOL_HUNTER,
        CardDb::Id::MURLOC_SCOUT_T_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SATED_THRESHADON,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, VulgarHomunculusNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::VULGAR_HOMUNCULUS,
        CardDb::Id::VULGAR_HOMUNCULUS_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, VulgarHomunculusProc) {
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

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);
    int health_before = arena.playerA().total_health();
    BattleReport report = arena.battle(true);
    int health_after = arena.playerA().total_health();

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);

    EXPECT_EQ(health_before - health_after, 4);
}

TEST(ArenaBattleTest, WrathWeaverNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::WRATH_WEAVER,
        CardDb::Id::WRATH_WEAVER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    int health_before = arena.playerA().total_health();

    // simulate playing a non-demon minion
    Minion minion = db.get_minion(CardDb::Id::HARMLESS_BONEHEAD);
    boardA.proc_trigger(Keyword::ON_PLAY, &minion);

    BattleReport report = arena.battle(true);
    int health_after = arena.playerA().total_health();

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);

    EXPECT_EQ(health_before - health_after, 0);
}

TEST(ArenaBattleTest, WrathWeaverProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::WRATH_WEAVER,
        CardDb::Id::WRATH_WEAVER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    int health_before = arena.playerA().total_health();

    // simulate playing a demon minion
    Minion minion = db.get_minion(CardDb::Id::VULGAR_HOMUNCULUS);
    arena.playerA().board().proc_trigger(Keyword::ON_PLAY, &minion);

    BattleReport report = arena.battle(true);
    int health_after = arena.playerA().total_health();

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 1);

    EXPECT_EQ(health_before - health_after, 3);
}

TEST(ArenaBattleTest, ColdlightSeer) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::COLDLIGHT_SEER_G,
        CardDb::Id::MURLOC_SCOUT_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}
