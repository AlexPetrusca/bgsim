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
        boardA.play_minion(minion);
    }

    Board boardB;
    for (int i = 0; i < 7; i++) {
        Minion minion = Minion("B" + std::to_string(i), 1, 5, 150);
        boardB.play_minion(minion);
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

    rng.seed(123456);
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

TEST(ArenaBattleTest, HarvestGolem) {
    Board boardA = Board::from_ids({
        CardDb::Id::HARVEST_GOLEM,
        CardDb::Id::HARVEST_GOLEM_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, KaboomBot) {
    Board boardA = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KaboomBotGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::KABOOM_BOT_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KaboomBotChain) {
    Board boardA = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KaboomBotChainLong) {
    Board boardA = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KaboomBotChainDivineShield) {
    Board boardA = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::PSYCH_O_TRON
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KaboomBotCleave) {
    Board boardA = Board::from_ids({
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
        CardDb::Id::KABOOM_BOT,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000_G
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MetaltoothLeaper) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::METALTOOTH_LEAPER_G,
        CardDb::Id::DAMAGED_GOLEM_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::MURLOC_WARLEADER_G,
        CardDb::Id::MURLOC_WARLEADER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    Minion& houndmaster = *arena.playerB().board().minions().begin();
    EXPECT_EQ(houndmaster.attack(), 8);
    EXPECT_EQ(houndmaster.health(), 6);

    BattleReport report = arena.battle(true);
    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MountedRaptor) {
    Board boardA = Board::from_ids({
        CardDb::Id::MOUNTED_RAPTOR,
        CardDb::Id::MOUNTED_RAPTOR_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, NathrezimOverseerNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::NATHREZIM_OVERSEER_G,
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::HOUNDMASTER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, NathrezimOverseerProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::NATHREZIM_OVERSEER_G,
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::IMP_T_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, OldMurkEyeUnbuffed) {
    Board boardA = Board::from_ids({
        CardDb::Id::OLD_MURK_EYE,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    EXPECT_FALSE(arena.playerA().board().triggers().empty());

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 3);

    EXPECT_TRUE(arena.playerA().board().triggers().empty());
}

TEST(ArenaBattleTest, OldMurkEyeBuffed) {
    Board boardA = Board::from_ids({
        CardDb::Id::OLD_MURK_EYE,
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::PRIMALFIN_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    EXPECT_FALSE(arena.playerA().board().triggers().empty());

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);

    EXPECT_TRUE(arena.playerA().board().triggers().empty());
    for (const auto& [keyword, effect]: arena.playerA().board().triggers()) {
        std::cout << static_cast<int>(keyword) << std::endl;
    }
}

TEST(ArenaBattleTest, OldMurkEyeGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::OLD_MURK_EYE_G,
        CardDb::Id::PRIMALFIN_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    EXPECT_FALSE(arena.playerA().board().triggers().empty());

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 1);

    EXPECT_TRUE(arena.playerA().board().triggers().empty());
}

TEST(ArenaBattleTest, PackLeaderNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::PACK_LEADER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, PackLeaderProcOnDeathrattle) {
    Board boardA = Board::from_ids({
        CardDb::Id::SEWER_RAT,
        CardDb::Id::PACK_LEADER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, PackLeaderProcOnSummon) {
    Board boardA = Board::from_ids({
        CardDb::Id::PACK_LEADER_G,
    });
    boardA.summon_minion(db.get_minion(CardDb::Id::ALLEYCAT_G), boardA.minions().begin());

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

TEST(ArenaBattleTest, PogoHopperNoProc) {
    Board boardA = Board();

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    arena.playerA().board().add_minion(db.get_minion(CardDb::Id::POGO_HOPPER));
    arena.playerA().board().add_minion(db.get_minion(CardDb::Id::POGO_HOPPER_G));
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, PogoHopperProcOnBattlecry) {
    Board boardA = Board();

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    arena.playerA().board().play_minion(db.get_minion(CardDb::Id::POGO_HOPPER)); // 1/1
    arena.playerA().board().play_minion(db.get_minion(CardDb::Id::POGO_HOPPER)); // 3/3
    arena.playerA().board().play_minion(db.get_minion(CardDb::Id::POGO_HOPPER_G)); // 10/10
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Zoobot) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::ZOOBOT,
        CardDb::Id::NIGHTMARE_AMALGAM,
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::HYENA_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ZoobotShouldOnlyHitOne) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::ZOOBOT,
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::PRIMALFIN_T,
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

TEST(ArenaBattleTest, ZoobotShouldntStack) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::ZOOBOT,
        CardDb::Id::NIGHTMARE_AMALGAM,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ZoobotShouldBuffAmalgamLast) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::ZOOBOT,
        CardDb::Id::HYENA_T,
        CardDb::Id::NIGHTMARE_AMALGAM,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, AnnoyOModule) {
    Board boardA = Board::from_ids({
        CardDb::Id::ANNOY_O_MODULE,
        CardDb::Id::ANNOY_O_MODULE_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, AnnoyOModulePlayNoMagnetize) {
    Board boardA = Board::from_ids({
        CardDb::Id::JO_E_BOT_T_G
    });
    boardA.play_minion(db.get_minion(CardDb::Id::ANNOY_O_MODULE));

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, AnnoyOModulePlayMagnetize) {
    Board boardA = Board::from_ids({
        CardDb::Id::JO_E_BOT_T_G
    });
    boardA.play_minion(db.get_minion(CardDb::Id::ANNOY_O_MODULE), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, AnnoyOModulePlayMagnetizeFullBoard) {
    Board boardA = Board::from_ids({
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::JO_E_BOT_T_G,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::ANNOY_O_MODULE), ++boardA.minions().begin());

    EXPECT_EQ(boardA.taunt_count(), 1);

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, CobaltGuardian) {
    Board boardA = Board::from_ids({
        CardDb::Id::COBALT_GUARDIAN,

    });
    boardA.play_minion(db.get_minion(CardDb::Id::MECHAROO_G));
    boardA.play_minion(db.get_minion(CardDb::Id::ANNOY_O_MODULE), ++boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
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

TEST(ArenaBattleTest, CobaltGuardianGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::COBALT_GUARDIAN_G,

    });
    boardA.play_minion(db.get_minion(CardDb::Id::MECHAROO));
    boardA.play_minion(db.get_minion(CardDb::Id::ANNOY_O_MODULE), ++boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::PACK_LEADER_G,
        CardDb::Id::PACK_LEADER_G,
        CardDb::Id::PACK_LEADER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, CrowdFavorite) {
    Board boardA = Board::from_ids({
        CardDb::Id::CROWD_FAVORITE,
        CardDb::Id::CROWD_FAVORITE_G,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::ALLEYCAT_G));
    boardA.play_minion(db.get_minion(CardDb::Id::ALLEYCAT_G));

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Crystalweaver) {
    Board boardA;
    boardA = Board::from_ids({
        CardDb::Id::CRYSTALWEAVER_G,
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::CRYSTALWEAVER,
        CardDb::Id::IMP_T_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, FloatingWatcher) {
    Board boardA = Board::from_ids({
        CardDb::Id::WRATH_WEAVER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    arena.playerA().board().play_minion(db.get_minion(CardDb::Id::FLOATING_WATCHER_G));
    arena.playerA().board().play_minion(db.get_minion(CardDb::Id::FLOATING_WATCHER));

    MinionLoc golden = ++arena.playerA().board().minions().begin();
    EXPECT_EQ(golden->attack(), 16);
    EXPECT_EQ(golden->health(), 16);

    MinionLoc normal = ++golden;
    EXPECT_EQ(normal->attack(), 6);
    EXPECT_EQ(normal->health(), 6);

    EXPECT_EQ(arena.playerA().total_health(), 38);

    BattleReport report = arena.battle(true);
    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, InfestedWolf) {
    Board boardA = Board::from_ids({
        CardDb::Id::INFESTED_WOLF,
        CardDb::Id::INFESTED_WOLF_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, IronSensei) {
    Board boardA = Board::from_ids({
        CardDb::Id::IRON_SENSEI,
        CardDb::Id::IRON_SENSEI_G,
    });
    boardA.proc_trigger(Keyword::ON_TURN_END);

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, IronSenseiNoSelfBuff) {
    Board boardA = Board::from_ids({
        CardDb::Id::IRON_SENSEI,
        CardDb::Id::MECHAROO_G,
    });
    boardA.proc_trigger(Keyword::ON_TURN_END);
    boardA.proc_trigger(Keyword::ON_TURN_END);
    boardA.proc_trigger(Keyword::ON_TURN_END);

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, Khadgar) {
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::KHADGAR,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, GoldenKhadgar) {
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::KHADGAR_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
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

TEST(ArenaBattleTest, DoubleKhadgar) {
    Board boardA = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::KHADGAR,
        CardDb::Id::KHADGAR,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KhadgarNoProcOnPlay) {
    Board boardA = Board::from_ids({
        CardDb::Id::KHADGAR,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::HARMLESS_BONEHEAD), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KhadgarProcOnBattlecry) {
    Board boardA = Board::from_ids({
        CardDb::Id::KHADGAR,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::ALLEYCAT), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, KhadgarSecurityRoverOverflow) {
    Board boardA = Board::from_ids({
        CardDb::Id::SECURITY_ROVER_G,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::KHADGAR_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::SAVANNAH_HIGHMANE_G,
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, PhalanxCommanderNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::ALLEYCAT_G,
        CardDb::Id::ALLEYCAT_G,
        CardDb::Id::PHALANX_COMMANDER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_B);
    EXPECT_EQ(report.damage(), 1);
}

TEST(ArenaBattleTest, PhalanxCommanderProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::SEWER_RAT,
        CardDb::Id::SEWER_RAT,
        CardDb::Id::PHALANX_COMMANDER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, PilotedShredder) {
    Board boardA = Board::from_ids({
        CardDb::Id::PILOTED_SHREDDER,
        CardDb::Id::PILOTED_SHREDDER_G
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RatPack) {
    Board boardA = Board::from_ids({
        CardDb::Id::RAT_PACK,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, RatPackGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::RAT_PACK_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, ReplicatingMenace) {
    Board boardA = Board::from_ids({
        CardDb::Id::REPLICATING_MENACE,
        CardDb::Id::REPLICATING_MENACE_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, ReplicatingMenaceMagnetize) {
    Board boardA = Board::from_ids({
        CardDb::Id::JO_E_BOT_T
    });
    boardA.play_minion(db.get_minion(CardDb::Id::REPLICATING_MENACE), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ReplicatingMenaceMagnetizeMultipleDeathrattle) {
    Board boardA = Board::from_ids({
        CardDb::Id::MECHAROO
    });
    boardA.play_minion(db.get_minion(CardDb::Id::REPLICATING_MENACE), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ScrewjankClunker) {
    Board boardA = Board::from_ids({
        CardDb::Id::SCREWJANK_CLUNKER,
        CardDb::Id::RYLAK_METALHEAD,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ShifterZerus) {
    Board boardA = Board::from_ids({
        CardDb::Id::SHIFTER_ZERUS,
        CardDb::Id::SHIFTER_ZERUS_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SoulJuggler) {
    Board boardA = Board::from_ids({
        CardDb::Id::VOIDLORD,
        CardDb::Id::VOIDWALKER,
        CardDb::Id::SOUL_JUGGLER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SoulJugglerGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::VOIDLORD,
        CardDb::Id::VOIDWALKER,
        CardDb::Id::SOUL_JUGGLER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::SAVANNAH_HIGHMANE,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, TheBeast) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BEAST_G,
        CardDb::Id::THE_BEAST,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    rng.seed(123456);
    Board boardB = Board::from_ids({
        CardDb::Id::SAVANNAH_HIGHMANE_G,
        CardDb::Id::SAVANNAH_HIGHMANE_G,
    });

    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, TheBeastMirror) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BEAST,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::THE_BEAST,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, TheBeastGoldenMirror) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BEAST_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::THE_BEAST_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, TheBeastInteractKhadgar) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BEAST,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::KHADGAR
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, TortollanShellraiser) {
    Board boardA = Board::from_ids({
        CardDb::Id::TORTOLLAN_SHELLRAISER,
        CardDb::Id::MURLOC_WARLEADER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, AnnihilanBattlemaster) {
    Board boardA = Board::from_ids({
        CardDb::Id::ANNIHILAN_BATTLEMASTER,
        CardDb::Id::ANNIHILAN_BATTLEMASTER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, AnnihilanBattlemasterProcBattlecry) {
    Board boardA = Board::from_ids({
        CardDb::Id::ANNIHILAN_BATTLEMASTER,
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::ANNIHILAN_BATTLEMASTER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    arena.playerA().set_health(15);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, BolvarFireblood) {
    Board boardA = Board::from_ids({
        CardDb::Id::BOLVAR_FIREBLOOD,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::PHALANX_COMMANDER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, BolvarFirebloodGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::BOLVAR_FIREBLOOD_G,
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

TEST(ArenaBattleTest, BolvarFirebloodProcOther) {
    Board boardA = Board::from_ids({
        CardDb::Id::SHIELDED_MINIBOT,
        CardDb::Id::BOLVAR_FIREBLOOD,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::PHALANX_COMMANDER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, CaveHydra) {
    Board boardA = Board::from_ids({
        CardDb::Id::CAVE_HYDRA,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, CaveHydraGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::CAVE_HYDRA_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::IMP_GANG_BOSS,
        CardDb::Id::SEWER_RAT,
        CardDb::Id::IMP_GANG_BOSS,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, DefenderOfArgus) {
    Board boardA = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::DEFENDER_OF_ARGUS), ++boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::SKELETON_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, DefenderOfArgusGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::DEFENDER_OF_ARGUS_G), ++boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, DefenderOfArgusWithRylakMetalhead) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::DEFENDER_OF_ARGUS,
        CardDb::Id::SKELETON_T_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::BIG_BAD_WOLF_T,
        CardDb::Id::BIG_BAD_WOLF_T,
        CardDb::Id::BIG_BAD_WOLF_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, JunkbotNoProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::SEWER_RAT_G,
        CardDb::Id::JUNKBOT_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, JunkbotProc) {
    Board boardA = Board::from_ids({
        CardDb::Id::ANNOY_O_TRON,
        CardDb::Id::JUNKBOT,
        CardDb::Id::JUNKBOT_G,
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

TEST(ArenaBattleTest, KingBagurgle) {
    Board boardA = Board::from_ids({
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::KING_BAGURGLE), boardA.minions().begin());

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

TEST(ArenaBattleTest, KingBagurgleGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
        CardDb::Id::SKELETON_T,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::KING_BAGURGLE_G), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
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

TEST(ArenaBattleTest, MamaBear) {
    Board boardA = Board::from_ids({
        CardDb::Id::MAMA_BEAR,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::SEWER_RAT), boardA.minions().begin());
    boardA.play_minion(db.get_minion(CardDb::Id::SEWER_RAT), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MamaBearGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::MAMA_BEAR_G,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::NIGHTMARE_AMALGAM), boardA.minions().begin());
    boardA.play_minion(db.get_minion(CardDb::Id::SEWER_RAT), boardA.minions().begin());

    Board boardB = Board::from_ids({
        CardDb::Id::FOE_REAPER_4000,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
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

TEST(ArenaBattleTest, MechanoEgg) {
    Board boardA = Board::from_ids({
        CardDb::Id::MECHANO_EGG,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MechanoEggGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::MECHANO_EGG_G,
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

TEST(ArenaBattleTest, MechanoEggMirroTie) {
    Board boardA = Board::from_ids({
        CardDb::Id::MECHANO_EGG_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::MECHANO_EGG
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MenagerieMagician) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::MENAGERIE_MAGICIAN,
        CardDb::Id::NIGHTMARE_AMALGAM,
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::HYENA_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MenagerieMagicianGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::MENAGERIE_MAGICIAN_G,
        CardDb::Id::NIGHTMARE_AMALGAM,
        CardDb::Id::PRIMALFIN_T,
        CardDb::Id::HYENA_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, PilotedSkyGolem) {
    Board boardA = Board::from_ids({
        CardDb::Id::PILOTED_SKY_GOLEM,
        CardDb::Id::PILOTED_SKY_GOLEM_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Siegebreaker) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMP_T,
        CardDb::Id::IMP_T,
        CardDb::Id::IMP_T,
        CardDb::Id::IMP_T,
        CardDb::Id::IMP_T,
        CardDb::Id::IMP_T,
        CardDb::Id::SIEGEBREAKER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SiegebreakerGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMP_T_G,
        CardDb::Id::IMPRISONER_G,
        CardDb::Id::SIEGEBREAKER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, StrongshellScavenger) {
    Board boardA = Board::from_ids({
        CardDb::Id::ANNOY_O_TRON,
        CardDb::Id::ANNOY_O_TRON,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::STRONGSHELL_SCAVENGER));

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T,
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

TEST(ArenaBattleTest, StrongshellScavengerGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::ANNOY_O_TRON,
        CardDb::Id::ANNOY_O_TRON,
    });
    boardA.play_minion(db.get_minion(CardDb::Id::STRONGSHELL_SCAVENGER_G));

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER,
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

TEST(ArenaBattleTest, TheBoogeymonster) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BOOGEYMONSTER
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, TheBoogeymonsterGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BOOGEYMONSTER_G
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, TheBoogeymonsterGoldenOutheal) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BOOGEYMONSTER_G
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 4);
}

TEST(ArenaBattleTest, TheBoogeymonsterProcsOnlyOnOwnKills) {
    Board boardA = Board::from_ids({
        CardDb::Id::THE_BOOGEYMONSTER,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
        CardDb::Id::SKELETON_T_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, VirmenSensei) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::VIRMEN_SENSEI,
        CardDb::Id::HYENA_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::HARMLESS_BONEHEAD,
        CardDb::Id::HARMLESS_BONEHEAD,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, VirmenSenseiGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD,
        CardDb::Id::VIRMEN_SENSEI_G,
        CardDb::Id::HYENA_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HARMLESS_BONEHEAD_G,
        CardDb::Id::SKELETON_T,
        CardDb::Id::HARMLESS_BONEHEAD,
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

TEST(ArenaBattleTest, IronhideDirehorn) {
    Board boardA = Board::from_ids({
        CardDb::Id::IRONHIDE_DIREHORN,
        CardDb::Id::SKELETON_T,
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

TEST(ArenaBattleTest, IronhideDirehornGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::IRONHIDE_DIREHORN_G,
        CardDb::Id::SKELETON_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
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

TEST(ArenaBattleTest, FesterootHulk) {
    Board boardA = Board::from_ids({
        CardDb::Id::FESTEROOT_HULK,
        CardDb::Id::SKELETON_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, FesterootHulkOtherAttacks) {
    Board boardA = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::FESTEROOT_HULK,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, FesterootHulkGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::SKELETON_T,
        CardDb::Id::FESTEROOT_HULK_G,
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

// todo: However, a health-granting aura such as Mal'Ganis can save a minion from dying, even if it enters play the same Phase the other minion was mortally wounded, because the aura recalculation is done before the Death Creation Step.
//  - Need a test to cover this.

TEST(ArenaBattleTest, Malganis) {
    Board boardA = Board::from_ids({
        CardDb::Id::FLOATING_WATCHER_G
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);

    Board& board = arena.playerA().board();
    board.play_minion(db.get_minion(CardDb::Id::MALGANIS), board.minions().begin());

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MalganisGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMP_T_G,
        CardDb::Id::IMP_T_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    Board& board = arena.playerA().board();
    board.play_minion(db.get_minion(CardDb::Id::MALGANIS_G), board.minions().begin());

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MalganisImmuneHero) {
    Board boardA = Board::from_ids({
        CardDb::Id::WRATH_WEAVER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);

    int health_before = arena.playerA().total_health();
    Board& board = arena.playerA().board();
    board.play_minion(db.get_minion(CardDb::Id::MALGANIS));
    board.play_minion(db.get_minion(CardDb::Id::FLOATING_WATCHER));

    EXPECT_EQ(arena.playerA().total_health(), health_before);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, MalganisNoDeathsOnUnapplyAura) {
    Board boardA = Board::from_ids({
        CardDb::Id::IMP_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);

    Board& board = arena.playerA().board();
    Minion minion = db.get_minion(CardDb::Id::MALGANIS_G);
    minion.props().set(Keyword::TAUNT);
    board.play_minion(minion, board.minions().begin());

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), WIN_A);
    EXPECT_EQ(report.damage(), 1);
    EXPECT_EQ(board.minions().begin()->health(), 1);
}

TEST(ArenaBattleTest, Maexxna) {
    Board boardA = Board::from_ids({
        CardDb::Id::MAEXXNA,
        CardDb::Id::MAEXXNA_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SECURITY_ROVER_G,
        CardDb::Id::SECURITY_ROVER_G,
        CardDb::Id::SECURITY_ROVER_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Toxfin) {
    Board boardA = Board::from_ids({
        CardDb::Id::TOXFIN,
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::TOXFIN_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::THE_BOOGEYMONSTER_G,
        CardDb::Id::THE_BOOGEYMONSTER_G,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SneedsOldShredder) {
    Board boardA = Board::from_ids({
        CardDb::Id::SNEEDS_OLD_SHREDDER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, SneedsOldShredderGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::SNEEDS_OLD_SHREDDER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, Ghastcoiler) {
    Board boardA = Board::from_ids({
        CardDb::Id::GHASTCOILER,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, GhastcoilerGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::GHASTCOILER_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER_G,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
        CardDb::Id::HOUNDMASTER,
    });

    rng.seed(123456);
    Arena arena = Arena::from_boards(boardA, boardB);

    Pool pool;
    arena.bind_pool(&pool);

    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ZappSlywick) {
    Board boardA = Board::from_ids({
        CardDb::Id::ZAPP_SLYWICK,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::IMP_T,
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::IMP_GANG_BOSS,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, ZappSlywickGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::ZAPP_SLYWICK_G,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::RYLAK_METALHEAD_G,
        CardDb::Id::IMP_GANG_BOSS_G,
        CardDb::Id::HYENA_T_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, GoldrinnTheGreatWolf) {
    Board boardA = Board::from_ids({
        CardDb::Id::GOLDRINN_THE_GREAT_WOLF,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
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

TEST(ArenaBattleTest, GoldrinnTheGreatWolfGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::GOLDRINN_THE_GREAT_WOLF_G,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
    });

    Board boardB = Board::from_ids({
        CardDb::Id::SAVANNAH_HIGHMANE_G,
        CardDb::Id::SAVANNAH_HIGHMANE_G,
        CardDb::Id::SAVANNAH_HIGHMANE_G,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}

TEST(ArenaBattleTest, LightfangEnforcer) {
    Board boardA = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::PRIMALFIN_T_G,
        CardDb::Id::IMP_T_G,
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::LIGHTFANG_ENFORCER,
    });
    boardA.proc_trigger(Keyword::ON_TURN_END);

    Board boardB = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
        CardDb::Id::HYENA_T_G,
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

TEST(ArenaBattleTest, LightfangEnforcerGolden) {
    Board boardA = Board::from_ids({
        CardDb::Id::HYENA_T_G,
        CardDb::Id::PRIMALFIN_T_G,
        CardDb::Id::PRIMALFIN_T_G,
        CardDb::Id::IMP_T_G,
        CardDb::Id::JO_E_BOT_T_G,
        CardDb::Id::LIGHTFANG_ENFORCER_G,
    });
    boardA.proc_trigger(Keyword::ON_TURN_END);

    Board boardB = Board::from_ids({
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::SAVANNAH_HIGHMANE,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
        CardDb::Id::HYENA_T,
    });

    rng.seed(12345);
    Arena arena = Arena::from_boards(boardA, boardB);
    BattleReport report = arena.battle(true);

    EXPECT_EQ(report.result(), TIE);
    EXPECT_EQ(report.damage(), 0);
}
