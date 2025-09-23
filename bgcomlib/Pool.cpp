#include "include/Pool.h"

Pool::Pool() {
    _tier_counts.reserve(7);
    for (int tier = 0; tier < _tiers.size(); tier++) {
        const int num_copies = _copies_by_tier[tier];
        for (CardDb::Id id : _tiers[tier]) {
            _pool[tier + 1][id] = num_copies;
            _total_count += num_copies;
            _tier_counts[tier] += num_copies;
        }
    }
}

CardDb::Id Pool::get_random_minionid_by_tier(const int tier) const {
    return CardDb::Id::ALLEYCAT;
}

const std::unordered_map<CardDb::Id, int>& Pool::get_tier(const int tier) const {
    return _pool.at(tier);
}

int Pool::total_count() const {
    return _total_count;
}

int Pool::tier_count(const int tier) const {
    return _tier_counts[tier - 1];
}

const std::vector<int>& Pool::tier_counts() const {
    return _tier_counts;
}
