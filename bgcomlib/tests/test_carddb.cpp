#include <gtest/gtest.h>

#include "card/CardDb.h"

TEST(CardDbTest, GetMinionFromDb) {
    CardDb db;
    const Minion minion = db.get_minion(96778);

    EXPECT_EQ(minion.name(), "Houndmaster");
    EXPECT_EQ(minion.attack(), 4);
    EXPECT_EQ(minion.health(), 3);
    EXPECT_EQ(minion.tier(), 3);
}
