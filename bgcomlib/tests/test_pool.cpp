#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "Pool.h"

TEST(PoolTest, InitializedProperly) {
    const Pool pool;
    for (int t = 1; t <= 7; t++) {
        const std::unordered_map<CardDb::Id, int>& tier = pool.get_tier(t);
        for (const auto& [id, count] : tier) {
            const Minion& minion = db.get_minion(id);
            EXPECT_EQ(minion.tier(), t);
            EXPECT_EQ(std::min(15, 19 - 2 * t), count);
        }
    }
}
