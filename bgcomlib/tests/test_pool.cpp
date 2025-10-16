#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include "Pool.h"
#include "util/Random.h"

#define EXPECT_APPROX_EQ(val1, val2, error) \
    EXPECT_LE(std::abs(1.0 - ((double)(val1)) / ((double)(val2))), (error))

static constexpr int ITERATIONS = 50000;
static constexpr float CI = 0.15;

TEST(PoolTest, InitializedProperly) {
    Pool pool;
    int computed_total_count = 0;
    for (int t = 1; t <= 7; t++) {
        const std::vector<CardDb::Id>& tier_ids = Pool::get_tier(t);
        int tier_copies = Pool::get_copies_for_tier(t);
        for (const CardDb::Id id : tier_ids) {
            Minion minion = db.get_minion(id);
            EXPECT_EQ(minion.tier(), t) << "(id: " << static_cast<int>(id) << ", name: "<< minion.name() << ")";
            EXPECT_EQ(tier_copies, pool.card_count(id));
        }
        EXPECT_EQ(pool.tier_count(t), tier_ids.size() * tier_copies);
        computed_total_count += pool.tier_count(t);
    }
    EXPECT_EQ(pool.total_count(), computed_total_count);
}

TEST(PoolTest, RandomSampleFromTier) {
    Pool pool;
    std::unordered_map<CardDb::Id, int> counts;
    for (int i = 0; i < ITERATIONS; i++) {
        counts[pool.get_random_from_tier(1)]++;
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
    Pool pool;
    std::unordered_map<int, int> tier_counts;
    std::unordered_map<CardDb::Id, int> id_counts;
    for (int i = 0; i < ITERATIONS; i++) {
        CardDb::Id id = pool.fetch(6);
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
        for (const CardDb::Id id : Pool::get_tier(tier)) {
            std::cout << static_cast<int>(id) << " \t-\t" << id_counts[id] << std::endl;
            EXPECT_APPROX_EQ(id_counts[id], expected, CI);
        }
        std::cout << std::endl;
    }

    // check tier probabilities
    for (int t = 1; t <= 6; t++) {
        const int actual = tier_counts.at(t);
        const int expected = ITERATIONS * static_cast<double>(pool.tier_count(t)) / pool.total_count();
        std::cout << "Tier " << t << ": " << 100 * expected << "% (Expected) vs " << 100 * actual << "% (Actual)" << std::endl;
        EXPECT_APPROX_EQ(actual, expected, CI);
    }
}

TEST(PoolTest, RandomSampleByRaceWithExclude) {
    Pool pool;
    std::unordered_map<CardDb::Id, int> counts;
    const Minion source = db.get_minion(CardDb::Id::PRIMALFIN_LOOKOUT_G);
    for (int i = 0; i < ITERATIONS; i++) {
        counts[pool.fetch_race(6, Race::MURLOC, &source)]++;
    }

    CardDb::Id exclude_id = static_cast<CardDb::Id>(source.alt_id());
    int total_count = pool.race_count_up_to(Race::MURLOC, 6) - pool.card_count(exclude_id);
    for (const auto& [id, count] : counts) {
        std::cout << static_cast<int>(id) << " \t-\t" << count << std::endl;
        if (id == exclude_id) {
            EXPECT_EQ(count, 0);
        } else {
            const int expected = ITERATIONS * static_cast<double>(pool.card_count(id)) / total_count;
            EXPECT_APPROX_EQ(count, expected, CI);
        }
    }

    EXPECT_EQ(counts[static_cast<CardDb::Id>(source.id())], 0);
}
