#include "include/Pool.h"

#include <iostream>

#include "util/Random.h"

Pool::Pool() {
    _tier_counts.reserve(7);
    for (int tier = 0; tier < _tiers.size(); tier++) {
        const int num_copies = _copies_by_tier[tier];
        for (CardDb::Id id : _tiers[tier]) {
            _pool[id] = num_copies;
            _total_count += num_copies;
            _tier_counts[tier] += num_copies;
        }
    }
}

CardDb::Id Pool::get_random_minionid_from_tier(const int tier) const {
    int select_idx = rng.rand_int(1, tier_count(tier));
    const std::vector<CardDb::Id>& ids = get_tier(tier);
    for (CardDb::Id id : ids) {
        select_idx -= _pool.at(id);
        if (select_idx <= 0) {
            return id;
        }
    }
    std::unreachable();
}

CardDb::Id Pool::get_random_minionid_up_to_tier(const int tier) const {
    int total_count = 0;
    for (int t = 1; t <= tier; t++) {
        total_count += _tier_counts[t - 1];
    }

    int select_idx = rng.rand_int(1, total_count);
    for (int t = 1; t <= tier; t++) {
        select_idx -= _tier_counts[t - 1];
        if (select_idx <= 0) {
            return get_random_minionid_from_tier(t);
        }
    }
    std::unreachable();
}

void Pool::take(const CardDb::Id id, const int count) {
    _pool[id] -= count;
    _total_count -= count;
    _tier_counts[db.get_minion(id).tier() - 1] -= count;
}

void Pool::put(const CardDb::Id id, const int count) {
    _pool[id] += count;
    _total_count += count;
    _tier_counts[db.get_minion(id).tier() - 1] += count;
}

int Pool::total_count() const {
    return _total_count;
}

int Pool::tier_count(const int tier) const {
    return _tier_counts[tier - 1];
}

int Pool::card_count(const CardDb::Id id) const {
    return _pool.at(id);
}

const std::vector<int>& Pool::tier_counts() const {
    return _tier_counts;
}

const std::vector<CardDb::Id>& Pool::get_tier(const int tier) {
    return _tiers[tier - 1];
}

int Pool::get_copies_for_tier(const int tier) {
    return _copies_by_tier[tier - 1];
}
