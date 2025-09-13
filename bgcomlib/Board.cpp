#include "include/Board.h"

#include <random>
#include <sstream>

#include "card/CardDb.h"

Board::Board(const std::vector<Minion>& minions) {
    if (minions.size() > 7) {
        throw std::invalid_argument("Too many minions: " + std::to_string(minions.size()));
    }
    for (const Minion& minion : minions) {
        add_minion(minion);
    }
}

Board Board::from_ids(const std::vector<CardDb::Id>& minionIds) {
    if (minionIds.size() > 7) {
        throw std::invalid_argument("Too many minions: " + std::to_string(minionIds.size()));
    }
    std::vector<Minion> minions;
    minions.reserve(minionIds.size());
    for (const CardDb::Id& minionId: minionIds) {
        minions.push_back(db.get_minion(minionId));
    }
    return Board(minions);
}

std::list<Minion>& Board::minions() {
    return this->_minions;
}

MinionLoc Board::get_random_minion_loc() {
    MinionLoc random_loc;
    std::uniform_real_distribution<> dist(0, 1);
    double count = 1;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->is_zombie()) continue;
        if (dist(_rng) < 1 / count) {
            random_loc = m;
        }
        count++;
    }
    return random_loc;
}

MinionLoc Board::get_random_minion_loc(const BitVector<Keyword>& exclude) {
    auto random_loc = minions().end();
    std::uniform_real_distribution<> dist(0, 1);
    double count = 1;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->is_zombie()) continue;
        if (m->props() & exclude) continue;
        if (dist(_rng) < 1 / count) {
            random_loc = m;
        }
        count++;

    }
    return random_loc;
}

MinionLoc Board::get_random_minion_loc_by_race(const Race race) {
    auto random_loc = minions().end();
    std::uniform_real_distribution<> dist(0, 1);
    double count = 0;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->is_zombie()) continue;
        if (!m->races().has(race)) continue;
        if (dist(_rng) < 1 / count) {
            random_loc = m;
        }
        count++;
    }
    return random_loc;
}

MinionLoc Board::get_left_minion_loc(const MinionLoc loc) {
    if (loc == minions().begin()) {
        return minions().end();
    }
    return std::prev(loc);
}

MinionLoc Board::get_right_minion_loc(const MinionLoc loc) {
    return std::next(loc);
}

bool Board::is_minion(const MinionLoc loc) {
    return loc != minions().end() && !loc->is_zombie();
}

void Board::add_minion(const Minion& minion) {
    add_minion(minion, _minions.end());
}

MinionLoc Board::add_minion(const Minion& minion, const MinionLoc loc) {
    const MinionLoc spawn_loc = _minions.insert(loc, minion);
    if (minion.has(Keyword::TAUNT)) {
        _taunt_count++;
    }
    for (const auto& keyword: minion.effects() | std::views::keys) {
        if (KeywordUtil::isTrigger(keyword)) {
            register_trigger(keyword, spawn_loc);
        }
    }
    return spawn_loc;
}

void Board::summon_minion(const Minion& minion, const bool post_death) {
    summon_minion(minion, _minions.end(), post_death);
}

void Board::summon_minion(const Minion& minion, const MinionLoc loc, const bool post_death) {
    if (full(!post_death)) return;
    const MinionLoc spawn_loc = add_minion(minion, loc);
    proc_trigger(Keyword::ON_SUMMON, spawn_loc);
}

void Board::enchant_minion(Minion& minion, const Enchantment& enchantment, const bool aura) {
    minion.props() |= enchantment.props();
    minion.delta_attack(enchantment.attack(), aura);
    minion.delta_health(enchantment.health(), aura);
}

void Board::enchant_random_minion(const Enchantment& enchantment) {
    // todo: hold on - its only a subset of props that I'm interested in filtering out, right?
    //  - `divine shield` and `reborn` should go to units that don't have them
    //  - but what about `taunt`... that shouldn't count right?
    //  - Maybe we first try to give the buff to a minion without the prop;
    //    Then if we can't, we just give it to any minion (even if it doesn't "do anything")
    const auto minion = get_random_minion_loc(enchantment.props());
    if (minion != minions().end()) {
        enchant_minion(*minion, enchantment);
    }
}

void Board::enchant_random_minion_by_race(const Enchantment& enchantment, const Race race) {
    // todo: can we really ignore props here? answer is NO (lol)
    const auto minion = get_random_minion_loc_by_race(race);
    if (minion != minions().end()) {
        enchant_minion(*minion, enchantment);
    }
}

bool Board::try_reap_minion(const MinionLoc loc) {
    const Minion& minion = *loc;
    if (minion.is_zombie()) {
        reap_minion(loc);
        return true;
    }
    return false;
}

void Board::reap_minion(const MinionLoc loc) {
    const Minion& minion = *loc;
    if (minion.has(Keyword::DEATHRATTLE)) {
        exec_effect(minion.get_effect(Keyword::DEATHRATTLE), loc);
    }
    if (minion.has(Keyword::REBORN)) {
        // todo: [optimize] so inefficient (maybe don't handle with effect)
        const Effect reborn_effect(Keyword::REBORN, Effect::Type::REBORN_SUMMON, {minion.id()});
        exec_effect(reborn_effect, loc);
    }
    if (minion.has(Keyword::TAUNT)) {
        _taunt_count--;
    }

    if (_active == loc) {
        increment_active();
    }
    deregister_triggers(loc);
    proc_trigger(Keyword::ON_DEATH_OTHER, loc);

    _minions.erase(loc); // erase zombie
    _zombie_count--;
}

int Board::damage_minion(const MinionLoc loc, const int damage, const bool poisoned) {
    Minion& minion = *loc;
    if (minion.has(Keyword::DIVINE_SHIELD)) {
        minion.clear(Keyword::DIVINE_SHIELD);
        return 0;
    } else {
        const int damage_dealt = minion.deal_damage(damage);
        if (minion.has(Keyword::ON_DAMAGE_SELF)) {
            exec_effect(minion.get_effect(Keyword::ON_DAMAGE_SELF), loc);
        }
        if (minion.health() <= 0 || poisoned) {
            minion.set_zombie(true);
            if (poisoned) {
                minion.set_poisoned(true);
            }
            _zombie_count++;
        }
        return damage_dealt;
    }
}

void Board::exec_effect(const Effect& effect, const MinionLoc loc) {
    const auto next_loc = std::next(loc);
    switch (effect.type()) {
        case Effect::Type::SUMMON: {
            for (const int minion_id: effect.args()) {
                const bool post_death = effect.trigger() == Keyword::DEATHRATTLE;
                summon_minion(db.get_minion(minion_id), next_loc, post_death);
            }
            break;
        }
        case Effect::Type::REBORN_SUMMON: {
            const int minion_id = effect.args()[0];
            Minion minion = db.get_minion(minion_id);
            minion.set_health(1);
            minion.clear(Keyword::REBORN);
            summon_minion(minion, next_loc, true);
            break;
        }
        case Effect::Type::ENCHANT: {
            for (const int enchantment_id: effect.args()) {
                Enchantment enchantment = db.get_enchantment(enchantment_id); // todo: don't copy
                switch (static_cast<CardDb::Id>(enchantment_id)) {
                    case CardDb::Id::GIVE_ATTACK_E: {
                        enchantment.set_attack(loc->attack());
                        break;
                    }
                    case CardDb::Id::GIVE_HEALTH_E: {
                        enchantment.set_health(loc->max_health());
                        break;
                    }
                    default:
                        break;
                }

                switch (enchantment.target()) {
                    case Target::SINGLE: {
                        if (enchantment.races().any()) {
                            // todo: this is wrong - Target::SINGLE should mean single target enchants
                            for (const Race race: enchantment.races()) {
                                enchant_random_minion_by_race(enchantment, race);
                            }
                        } else {
                            enchant_random_minion(enchantment);
                        }
                        break;
                    }
                    case Target::ALL: {
                        // todo: take race into account
                        for (auto & m : minions()) {
                            enchant_minion(m, enchantment);
                        }
                        break;
                    }
                    case Target::SELF: {
                        enchant_minion(*loc, enchantment);
                        break;
                    }
                    case Target::LEFTMOST: {
                        // todo: take race into account
                        enchant_minion(minions().front(), enchantment);
                        break;
                    }
                    case Target::RIGHTMOST: {
                        // todo: take race into account
                        enchant_minion(minions().back(), enchantment);
                        break;
                    }
                }
            }
            break;
        }
        case Effect::Type::GEN_CARD: {
            // todo
            break;
        }
        case Effect::Type::TRIGGER_ADJACENT_BATTLECRY: {
            const auto l = get_left_minion_loc(loc);
            const auto r = get_right_minion_loc(loc);
            const bool is_battlecry_left = is_minion(l) && l->has(Keyword::BATTLECRY);
            const bool is_battlecry_right = is_minion(r) && r->has(Keyword::BATTLECRY);
            if (is_battlecry_left && is_battlecry_right) {
                std::uniform_int_distribution coin_flip(0, 1);
                if (coin_flip(_rng)) {
                    exec_effect(l->get_effect(Keyword::BATTLECRY), l);
                } else {
                    exec_effect(r->get_effect(Keyword::BATTLECRY), r);
                }
            } else if (is_battlecry_left) {
                exec_effect(l->get_effect(Keyword::BATTLECRY), l);
            } else if (is_battlecry_right) {
                exec_effect(r->get_effect(Keyword::BATTLECRY), r);
            }
            break;
        }
        case Effect::Type::TRIGGER_ADJACENT_BATTLECRIES: {
            const auto l = get_left_minion_loc(loc);
            if (is_minion(l) && l->has(Keyword::BATTLECRY)) {
                exec_effect(l->get_effect(Keyword::BATTLECRY), l);
            }
            const auto r = get_right_minion_loc(loc);
            if (is_minion(r) && r->has(Keyword::BATTLECRY)) {
                exec_effect(r->get_effect(Keyword::BATTLECRY), r);
            }
            break;
        }
    }
}

void Board::apply_adjacent_aura(const MinionLoc loc) {
    const Effect& effect = loc->get_effect(Keyword::ADJACENT_AURA);
    for (const int enchantment_id: effect.args()) {
        const Enchantment& enchantment = db.get_enchantment(enchantment_id);
        const MinionLoc left = get_left_minion_loc(loc);
        if (left != minions().end()) {
            enchant_minion(*left, enchantment, true);
        }
        MinionLoc right = get_right_minion_loc(loc);
        if (right != minions().end()) {
            enchant_minion(*right, enchantment, true);
        }
    }
}

// void Board::undo_adjacent_aura(const MinionLoc loc) {
//     const Effect& effect = loc->get_effect(Keyword::ADJACENT_AURA);
//     for (const int enchantment_id: effect.args()) {
//         const Enchantment& enchantment = db.get_enchantment(enchantment_id);
//         const MinionLoc left = get_left_minion_loc(loc);
//         if (left != minions().end()) {
//             disenchant_minion(*left, enchantment);
//         }
//         MinionLoc right = get_right_minion_loc(loc);
//         if (right != minions().end()) {
//             disenchant_minion(*right, enchantment);
//         }
//     }
// }

void Board::pre_combat() {
    // todo: optimize - takes a huge amount of time
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        m->disable_aura();
    }
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->has(Keyword::ADJACENT_AURA)) {
            apply_adjacent_aura(m);
        }
    }
}

void Board::pre_battle() {
    _active = _minions.begin();
    // todo: when we clone the list, all of our iterators are invalidated - they point to the original list.
    //  - so we have to recompute them.
    //  - is there anything clever we can do about this?
    //  - we coid: override the copy constructor to do this
    _triggers.clear();
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        for (const auto& keyword: m->effects() | std::views::keys) {
            if (KeywordUtil::isTrigger(keyword)) {
                register_trigger(keyword, m);
            }
        }
    }
}

void Board::increment_active() {
    if (empty(true)) return;

    ++_active;
    if (_active == _minions.end()) {
        _active = _minions.begin(); // wraparound
    }
}

void Board::proc_trigger(const Keyword trigger, const MinionLoc source) {
    for (const MinionLoc listener : _triggers[trigger]) {
        const Effect& effect = listener->get_effect(trigger);
        if (Effect::ConstraintUtil::matchesRace(effect.constraint(), source->races())) {
            exec_effect(effect, listener);
        }
    }
}

void Board::register_trigger(const Keyword trigger, const MinionLoc loc) {
    _triggers[trigger].insert(loc);
}

void Board::deregister_trigger(const Keyword trigger, const MinionLoc loc) {
    _triggers[trigger].erase(loc);
}

void Board::deregister_triggers(const MinionLoc loc) {
    for (const auto& keyword: loc->effects() | std::views::keys) {
        if (KeywordUtil::isTrigger(keyword)) {
            deregister_trigger(keyword, loc);
        }
    }
}

MinionLoc Board::active() const {
    return _active;
}

int Board::tier_total() const {
    int total = 0;
    for (const Minion& minion: _minions) {
        total += minion.tier();
    }
    return total;
}

size_t Board::size(const bool include_zombies) const {
    if (include_zombies) {
        return _minions.size();
    } else {
        return _minions.size() - _zombie_count;
    }
}

bool Board::empty(const bool include_zombies) const {
    return size(include_zombies) == 0;
}

bool Board::full(const bool include_zombies) const {
    return size(include_zombies) >= 7;
}

void Board::set_rng(const std::mt19937& rng) {
    _rng = rng;
}

int Board::taunt_count() const {
    return _taunt_count;
}

int Board::zombie_count() const {
    return _zombie_count;
}

[[nodiscard]] std::string Board::to_string() {
    std::ostringstream oss;
    oss << _minions.size() << " | ";
    for (auto& minion: _minions) {
        oss << minion << " | ";
    }
    return oss.str();
}
