#include <gtest/gtest.h>

#include "card/CardDb.h"

TEST(CardDbTest, GetMinion) {
    CardDb db;
    const Minion minion = db.get_minion(96778); // Houndmaster
    EXPECT_EQ(minion.name(), "Houndmaster");
    EXPECT_EQ(minion.attack(), 4);
    EXPECT_EQ(minion.health(), 3);
    EXPECT_EQ(minion.tier(), 3);
}

TEST(CardDbTest, ResolveDeathrattles) {
    CardDb db;
    const Minion minion = db.get_minion(104551); // Harmless Bonehead
    EXPECT_EQ(minion.name(), "Harmless Bonehead");
    EXPECT_TRUE(minion.has_keyword(Keyword::DEATHRATTLE));

    const Effect& deathrattle = minion.get_effect(Keyword::DEATHRATTLE);
    EXPECT_TRUE(deathrattle.type() == Effect::Type::SUMMON);
    EXPECT_EQ(deathrattle.args().size(), 2);
    EXPECT_EQ(deathrattle.args().at(0), 99629);

    const Minion token = db.get_minion(99629); // Skeleton
    EXPECT_EQ(token.name(), "Skeleton");
}

TEST(CardDbTest, ResolveGoldenVersion) {
    CardDb db;
    const Minion minion = db.get_minion(96778); // Houndmaster
    EXPECT_EQ(minion.name(), "Houndmaster");
    EXPECT_FALSE(minion.is_golden());

    const Minion golden = db.get_minion(minion.alt_id()); // Golden Houndmaster
    EXPECT_EQ(golden.name(), "Houndmaster");
    EXPECT_TRUE(golden.is_golden());

    EXPECT_EQ(golden.alt_id(), minion.id());
    EXPECT_EQ(golden.health(), 2 * minion.health());
    EXPECT_EQ(golden.attack(), 2 * minion.attack());
    EXPECT_EQ(golden.tier(), minion.tier());
}
