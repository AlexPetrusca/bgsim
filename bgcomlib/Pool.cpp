#include "include/Pool.h"

Pool::Pool() {
    for (int tier = 0; tier < _tiers.size(); tier++) {
        const int tier_count = _counts[tier];
        for (CardDb::Id id : _tiers[tier]) {
            _pool[tier + 1][id] = tier_count;
        }
    }
}

const std::unordered_map<CardDb::Id, int>& Pool::get_tier(const int tier) const {
    return _pool.at(tier);
}

// const std::unordered_map<CardDb::Id, int>& Pool::get_random_minion(const int tier) const {
// }
