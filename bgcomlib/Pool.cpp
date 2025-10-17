#include "include/Pool.h"

#include <iostream>
#include <unordered_set>

#include "util/Random.h"

Pool::Pool() {
    _tier_counts.resize(7);
    _keywords.resize(7);
    _keyword_counts.resize(7);
    _races.resize(7);
    _race_counts.resize(7);

    _pool[CardDb::Id::NONE] = 0;
    for (int tier = 0; tier < _tiers.size(); tier++) {
        const int num_copies = _copies_by_tier[tier];
        for (const CardDb::Id id : _tiers[tier]) {
            const Minion& minion = db.get_minion(id);
            _pool[id] += num_copies;
            _total_count += num_copies;
            _tier_counts[db.get_minion(id).tier() - 1] += num_copies;
            for (const Keyword keyword : minion.props()) {
                _keywords[tier][keyword].push_back(id);
                _keyword_counts[tier][keyword] += num_copies;
            }
            for (const Race race : minion.races()) {
                _races[tier][race].push_back(id);
                _race_counts[tier][race] += num_copies;
            }
        }
    }
}

CardDb::Id Pool::get_random_from_tier(const int tier) {
    int select_idx = rng.rand_int(1, tier_count(tier));
    for (CardDb::Id id: get_tier(tier)) {
        select_idx -= _pool.at(id);
        if (select_idx <= 0) {
            return id;
        }
    }
    std::unreachable();
}

CardDb::Id Pool::fetch(const int tier, const Minion* source) {
    // CardDb::Id exclude_id = CardDb::Id::NONE;
    // if (source != nullptr && source->tier() <= tier) {
    //     if (source->is_golden()) {
    //         exclude_id = static_cast<CardDb::Id>(source->alt_id());
    //     } else {
    //         exclude_id = static_cast<CardDb::Id>(source->id());
    //     }
    // }
    //
    // int total_count = -_pool.at(exclude_id);
    // for (int t = 1; t <= tier; t++) {
    //     total_count += _tier_counts[t - 1];
    // }
    //
    // int select_tier = 0;
    // int select_idx = rng.rand_int(1, total_count);
    // for (int t = 1; t <= tier; t++) {
    //     select_idx -= _tier_counts[t - 1];
    //     if (select_idx <= 0) {
    //         select_tier = t;
    //         break;
    //     }
    // }
    //
    // int select_tier_count = _tier_counts[select_tier - 1];
    // if (source != nullptr && source->tier() == select_tier) {
    //     select_tier_count -= _pool.at(exclude_id);
    // }
    //
    // int select_idx2 = rng.rand_int(1, select_tier_count);
    // for (CardDb::Id id: _tiers[select_tier - 1]) {
    //     if (id == exclude_id) continue;
    //     select_idx2 -= _pool.at(id);
    //     if (select_idx2 <= 0) {
    //         return id;
    //     }
    // }
    // std::unreachable();

    CardDb::Id select;

    auto exclude_id = CardDb::Id::NONE;
    if (source != nullptr) {
        if (source->is_golden()) {
            exclude_id = static_cast<CardDb::Id>(source->alt_id());
        } else {
            exclude_id = static_cast<CardDb::Id>(source->id());
        }
    }

    double counter = 0;
    for (int t = 1; t <= tier; t++) {
        for (const CardDb::Id id: get_tier(t)) {
            if (id == exclude_id) continue;

            const int current_count = card_count(id);
            counter += current_count;
            if (rng.rand_percent() < current_count / counter) {
                select = id;
            }
        }
    }

    return select;
}

CardDb::Id Pool::fetch_race(const int tier, const Race race, const Minion* source) {
    // CardDb::Id exclude_id = CardDb::Id::NONE;
    // if (source != nullptr && source->tier() <= tier && source->is(race)) {
    //     if (source->is_golden()) {
    //         exclude_id = static_cast<CardDb::Id>(source->alt_id());
    //     } else {
    //         exclude_id = static_cast<CardDb::Id>(source->id());
    //     }
    // }
    //
    // int total_count = -_pool.at(exclude_id);
    // for (int t = 1; t <= tier; t++) {
    //     total_count += _race_counts[t - 1][race];
    // }
    //
    // int select_tier = 0;
    // int select_idx = rng.rand_int(1, total_count);
    // for (int t = 1; t <= tier; t++) {
    //     select_idx -= _race_counts[t - 1][race];
    //     if (select_idx <= 0) {
    //         select_tier = t;
    //         break;
    //     }
    // }
    //
    // int select_tier_count = _race_counts[select_tier - 1][race];
    // if (source != nullptr && source->is(race) && source->tier() == select_tier) {
    //     select_tier_count -= _pool.at(exclude_id);
    // }
    //
    // int select_idx2 = rng.rand_int(1, select_tier_count);
    // for (CardDb::Id id: _races[select_tier - 1][race]) {
    //     if (id == exclude_id) continue;
    //     select_idx2 -= _pool.at(id);
    //     if (select_idx2 <= 0) {
    //         return id;
    //     }
    // }
    // std::unreachable();

    CardDb::Id select;

    auto exclude_id = CardDb::Id::NONE;
    if (source != nullptr) {
        if (source->is_golden()) {
            exclude_id = static_cast<CardDb::Id>(source->alt_id());
        } else {
            exclude_id = static_cast<CardDb::Id>(source->id());
        }
    }

    double counter = 0;
    for (int t = 1; t <= tier; t++) {
        for (const CardDb::Id id: get_race(t, race)) {
            if (id == exclude_id) continue;

            const int current_count = card_count(id);
            counter += current_count;
            if (rng.rand_percent() < current_count / counter) {
                select = id;
            }
        }
    }

    return select;
}

CardDb::Id Pool::fetch_keyword(const int tier, const Keyword keyword, const Minion* source) {
    // CardDb::Id exclude_id = CardDb::Id::NONE;
    // if (source != nullptr && source->tier() <= tier && source->has(keyword)) {
    //     if (source->is_golden()) {
    //         exclude_id = static_cast<CardDb::Id>(source->alt_id());
    //     } else {
    //         exclude_id = static_cast<CardDb::Id>(source->id());
    //     }
    // }
    //
    // int total_count = -_pool.at(exclude_id);
    // for (int t = 1; t <= tier; t++) {
    //     total_count += _keyword_counts[t - 1][keyword];
    // }
    //
    // int select_tier = 0;
    // int select_idx = rng.rand_int(1, total_count);
    // for (int t = 1; t <= tier; t++) {
    //     select_idx -= _keyword_counts[t - 1][keyword];
    //     if (select_idx <= 0) {
    //         select_tier = t;
    //         break;
    //     }
    // }
    //
    // int select_tier_count = _keyword_counts[select_tier - 1][keyword];
    // if (source != nullptr && source->has(keyword) && source->tier() == select_tier) {
    //     select_tier_count -= _pool.at(exclude_id);
    // }
    //
    // int select_idx2 = rng.rand_int(1, select_tier_count);
    // for (CardDb::Id id: _keywords[select_tier - 1][keyword]) {
    //     if (id == exclude_id) continue;
    //     select_idx2 -= _pool.at(id);
    //     if (select_idx2 <= 0) {
    //         return id;
    //     }
    // }
    // std::unreachable();

    CardDb::Id select;

    auto exclude_id = CardDb::Id::NONE;
    if (source != nullptr) {
        if (source->is_golden()) {
            exclude_id = static_cast<CardDb::Id>(source->alt_id());
        } else {
            exclude_id = static_cast<CardDb::Id>(source->id());
        }
    }

    double counter = 0;
    for (int t = 1; t <= tier; t++) {
        for (const CardDb::Id id: get_keyword(t, keyword)) {
            if (id == exclude_id) continue;

            const int current_count = card_count(id);
            counter += current_count;
            if (rng.rand_percent() < current_count / counter) {
                select = id;
            }
        }
    }

    return select;
}

// todo: pretty slow - we are repeating reservoir sampling `count` times :(
//  - we could use the counts we keep track of to speed each iteration up

std::vector<CardDb::Id> Pool::discover(const int tier, const Minion* source, const int count) {
    std::vector<CardDb::Id> reservoir;
    reservoir.reserve(count);

    auto exclude_id = CardDb::Id::NONE;
    if (source != nullptr) {
        if (source->is_golden()) {
            exclude_id = static_cast<CardDb::Id>(source->alt_id());
        } else {
            exclude_id = static_cast<CardDb::Id>(source->id());
        }
    }
    std::unordered_set<CardDb::Id> excludes;
    excludes.insert(exclude_id);

    for (int i = 0; i < count; i++) {
        CardDb::Id select;

        double counter = 0;
        for (int t = 1; t <= tier; t++) {
            for (const CardDb::Id id: get_tier(t)) {
                if (excludes.contains(id)) continue;

                const int current_count = card_count(id);
                counter += current_count;
                if (rng.rand_percent() < current_count / counter) {
                    select = id;
                }
            }
        }

        reservoir.push_back(select);
        excludes.insert(select);
        take(select);
    }

    return reservoir;
}

std::vector<CardDb::Id> Pool::discover_race(const int tier, const Race race, const Minion* source, const int count) {
    std::vector<CardDb::Id> reservoir;
    reservoir.reserve(count);

    auto exclude_id = CardDb::Id::NONE;
    if (source != nullptr) {
        if (source->is_golden()) {
            exclude_id = static_cast<CardDb::Id>(source->alt_id());
        } else {
            exclude_id = static_cast<CardDb::Id>(source->id());
        }
    }
    std::unordered_set<CardDb::Id> excludes;
    excludes.insert(exclude_id);

    for (int i = 0; i < count; i++) {
        CardDb::Id select;

        double counter = 0;
        for (int t = 1; t <= tier; t++) {
            for (const CardDb::Id id: get_race(t, race)) {
                if (excludes.contains(id)) continue;

                const int current_count = card_count(id);
                counter += current_count;
                if (rng.rand_percent() < current_count / counter) {
                    select = id;
                }
            }
        }

        reservoir.push_back(select);
        excludes.insert(select);
        take(select);
    }

    return reservoir;
}

std::vector<CardDb::Id> Pool::discover_keyword(const int tier, const Keyword keyword, const Minion* source, const int count) {
    std::vector<CardDb::Id> reservoir;
    reservoir.reserve(count);

    auto exclude_id = CardDb::Id::NONE;
    if (source != nullptr) {
        if (source->is_golden()) {
            exclude_id = static_cast<CardDb::Id>(source->alt_id());
        } else {
            exclude_id = static_cast<CardDb::Id>(source->id());
        }
    }
    std::unordered_set<CardDb::Id> excludes;
    excludes.insert(exclude_id);

    for (int i = 0; i < count; i++) {
        CardDb::Id select;

        double counter = 0;
        for (int t = 1; t <= tier; t++) {
            for (const CardDb::Id id: get_keyword(t, keyword)) {
                if (excludes.contains(id)) continue;

                const int current_count = card_count(id);
                counter += current_count;
                if (rng.rand_percent() < current_count / counter) {
                    select = id;
                }
            }
        }

        reservoir.push_back(select);
        excludes.insert(select);
        take(select);
    }

    return reservoir;
}

void Pool::take(const CardDb::Id id, const int count) {
    const Minion& minion = db.get_minion(id);
    _pool[id] -= count;
    _total_count -= count;
    _tier_counts[minion.tier() - 1] -= count;
    for (const Keyword keyword : minion.props()) {
        _keyword_counts[minion.tier() - 1][keyword] -= count;
    }
    for (const Race race : minion.races()) {
        _race_counts[minion.tier() - 1][race] -= count;
    }
}

void Pool::put(const CardDb::Id id, const int count) {
    const Minion& minion = db.get_minion(id);
    _pool[id] += count;
    _total_count += count;
    _tier_counts[minion.tier() - 1] += count;
    for (const Keyword keyword : minion.props()) {
        _keyword_counts[minion.tier() - 1][keyword] += count;
    }
    for (const Race race : minion.races()) {
        _race_counts[minion.tier() - 1][race] += count;
    }
}

int Pool::total_count() {
    return _total_count;
}

int Pool::tier_count(const int tier) {
    return _tier_counts[tier - 1];
}

int Pool::tier_count_up_to(const int max_tier) {
    int count = 0;
    for (int t = 1; t <= max_tier; t++) {
        count += tier_count(t);
    }
    return count;
}

int Pool::race_count(const Race race, const int tier) {
    return _race_counts[tier - 1][race];
}

int Pool::race_count_up_to(const Race race, const int max_tier) {
    int count = 0;
    for (int t = 1; t <= max_tier; t++) {
        count += race_count(race, t);
    }
    return count;
}

int Pool::keyword_count(const Keyword keyword, const int tier) {
    return _keyword_counts[tier - 1][keyword];
}

int Pool::keyword_count_up_to(const Keyword keyword, const int max_tier) {
    int count = 0;
    for (int t = 1; t <= max_tier; t++) {
        count += keyword_count(keyword, t);
    }
    return count;
}

int Pool::card_count(const CardDb::Id id) {
    return _pool.at(id);
}

const std::vector<CardDb::Id>& Pool::get_tier(const int tier) {
    return _tiers[tier - 1];
}

const std::vector<CardDb::Id>& Pool::get_race(const int tier, const Race race) {
    return _races[tier - 1][race];
}

const std::vector<CardDb::Id>& Pool::get_keyword(const int tier, const Keyword keyword) {
    return _keywords[tier - 1][keyword];
}

int Pool::get_copies_for_tier(const int tier) {
    return _copies_by_tier[tier - 1];
}
