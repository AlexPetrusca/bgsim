#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "Pool.h"

TEST(PoolTest, InitializedProperly) {
    const Pool pool;
    int computed_total_count = 0;
    for (int t = 1; t <= 7; t++) {
        const std::unordered_map<CardDb::Id, int>& tier = pool.get_tier(t);
        int t_copies = std::min(15, 19 - 2 * t);
        for (const auto& [id, count] : tier) {
            const Minion& minion = db.get_minion(id);
            EXPECT_EQ(minion.tier(), t);
            EXPECT_EQ(t_copies, count);
        }
        EXPECT_EQ(pool.tier_count(t), tier.size() * t_copies);
        computed_total_count += pool.tier_count(t);
    }
    EXPECT_EQ(pool.total_count(), computed_total_count);
}
