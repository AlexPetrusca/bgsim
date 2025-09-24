#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "Pool.h"
#include "util/Random.h"

#define EXPECT_APPROX_EQ(val1, val2, error) \
    EXPECT_LE(std::abs(1.0 - ((double)(val1)) / ((double)(val2))), (error))

static constexpr int ITERATIONS = 250000;
static constexpr float CI = 0.05;

TEST(PoolTest, InitializedProperly) {
    const Pool pool;
    int computed_total_count = 0;
    for (int t = 1; t <= 7; t++) {
        const std::vector<CardDb::Id>& tier_ids = Pool::get_tier(t);
        int tier_copies = Pool::get_copies_for_tier(t);
        for (const CardDb::Id id : tier_ids) {
            EXPECT_EQ(db.get_minion(id).tier(), t);
            EXPECT_EQ(tier_copies, pool.card_count(id));
        }
        EXPECT_EQ(pool.tier_count(t), tier_ids.size() * tier_copies);
        computed_total_count += pool.tier_count(t);
    }
    EXPECT_EQ(pool.total_count(), computed_total_count);
}

TEST(PoolTest, RandomSampleFromTier) {
    const Pool pool;
    std::unordered_map<CardDb::Id, int> counts;
    for (int i = 0; i < ITERATIONS; i++) {
        counts[pool.get_random_minionid_from_tier(1)]++;
    }

    const int expected = ITERATIONS / static_cast<int>(Pool::get_tier(1).size());
    std::cout << "----------------" << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    std::cout << "----------------" << std::endl;
    for (const auto& [id, count] : counts) {
        std::cout << static_cast<int>(id) << " \t-\t" << count << std::endl;
        EXPECT_APPROX_EQ(count, expected, CI);
    }
}

TEST(PoolTest, RandomSampleUpToTier) {
    const Pool pool;
    std::unordered_map<int, int> tier_counts;
    std::unordered_map<CardDb::Id, int> id_counts;
    for (int i = 0; i < ITERATIONS; i++) {
        CardDb::Id id = pool.get_random_minionid_up_to_tier(6);
        tier_counts[db.get_minion(id).tier()]++;
        id_counts[id]++;
    }

    // check minion probabilities
    for (int tier = 1; tier <= 6; tier++) {
        const int total = tier_counts[tier];
        const int expected = total / static_cast<int>(Pool::get_tier(tier).size());
        std::cout << "----------------" << std::endl;
        std::cout << "Total: " << total << std::endl;
        std::cout << "Expected: " << expected << std::endl;
        std::cout << "----------------" << std::endl;
        const std::vector<CardDb::Id>& ids = Pool::get_tier(tier);
        for (const CardDb::Id id : ids) {
            std::cout << static_cast<int>(id) << " \t-\t" << id_counts[id] << std::endl;
            EXPECT_APPROX_EQ(id_counts[id], expected, CI);
        }
        std::cout << std::endl;
    }

    // check tier probabilities
    for (int t = 1; t <= 6; t++) {
        double actual = static_cast<double>(tier_counts.at(t)) / ITERATIONS;
        double expected = static_cast<double>(pool.tier_count(t)) / pool.total_count();
        std::cout << "Tier " << t << ": " << 100 * expected << "% (Expected) vs " << 100 * actual << "% (Actual)" << std::endl;
        EXPECT_APPROX_EQ(actual, expected, CI);
    }
}
