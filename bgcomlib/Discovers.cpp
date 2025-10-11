#include "Discovers.h"

#include <csignal>

Discovers::Discovers() {
}

// todo: is having to pass tier kinda annoying? Should this just have a reference to the player
void Discovers::discover(const Effect::Discover type, const int tier, const Minion* source) {
    switch (type) {
        case Effect::Discover::TIER_1:
        case Effect::Discover::TIER_2:
        case Effect::Discover::TIER_3:
        case Effect::Discover::TIER_4:
        case Effect::Discover::TIER_5:
        case Effect::Discover::TIER_6:
        case Effect::Discover::TIER_7: {
            const int discover_tier = static_cast<int>(type);
            for (const CardDb::Id id : _pool->discover(discover_tier, source, 3)) {
                add_card(id);
            }
            break;
        }
        case Effect::Discover::BEAST:
        case Effect::Discover::DEMON:
        case Effect::Discover::DRAGON:
        case Effect::Discover::ELEMENTAL:
        case Effect::Discover::MECHANICAL:
        case Effect::Discover::MURLOC:
        case Effect::Discover::NAGA:
        case Effect::Discover::PIRATE:
        case Effect::Discover::QUILLBOAR:
        case Effect::Discover::UNDEAD: {
            const Race race = Effect::DiscoverUtil::to_race(type);
            for (const CardDb::Id id : _pool->discover_race(tier, race, source, 3)) {
                add_card(id);
            }
            break;
        }
        case Effect::Discover::BATTLECRY:
        case Effect::Discover::DEATHRATTLE: {
            const Keyword keyword = Effect::DiscoverUtil::to_keyword(type);
            for (const CardDb::Id id : _pool->discover_keyword(tier, keyword, source, 3)) {
                add_card(id);
            }
            break;
        }
    }
}

Minion Discovers::select(const int idx) {
    assert(idx < _cards.size() && "Out of bounds!");

    MinionLoc loc = _cards.begin();
    for (int i = 0; i < idx; i++) {
        ++loc;
    }
    Minion minion = *loc;

    for (int i = 0; i < 3; i++) {
        if (i != idx) {
            _pool->put(static_cast<CardDb::Id>(_cards.front().id())); // todo: we should have another id method to avoid static_cast
        }
        _cards.pop_front();
    }

    return minion;
}

void Discovers::add_card(const Minion& minion) {
    _cards.push_back(minion);
}

void Discovers::add_card(const CardDb::Id id) {
    add_card(db.get_minion(id));
}

void Discovers::remove_card(const MinionLoc loc) {
    _cards.erase(loc);
}

void Discovers::remove_card(const int idx) {
    if (idx >= _cards.size()) return;

    MinionLoc loc = _cards.begin();
    for (int i = 0; i < idx; i++) {
        ++loc;
    }
    remove_card(loc);
}

MinionLoc Discovers::begin() {
    return _cards.begin();
}

MinionLoc Discovers::end() {
    return _cards.end();
}

std::list<Minion>& Discovers::cards() {
    return _cards;
}

int Discovers::size() {
    return _cards.size();
}

void Discovers::bind_pool(Pool* pool) {
    _pool = pool;
}
