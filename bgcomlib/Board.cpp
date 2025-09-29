#include "include/Board.h"

#include <iostream>
#include <random>
#include <sstream>

#include "Player.h"
#include "card/CardDb.h"
#include "util/Random.h"

Board::Board(const std::vector<Minion>& minions) {
    _player = nullptr;
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

Race Board::get_random_race() {
    return get_random_race(RaceUtil::races);
}

Race Board::get_random_race(const std::vector<Race>& races) {
    Race race = Race::NONE;
    double count = 1;
    for (const Race r : races) {
        if (rng.rand_percent() < 1 / count) {
            race = r;
        }
        count++;
    }
    return race;
}

Race Board::get_random_race(const BitVector<Race>& races) {
    Race race = Race::NONE;
    double count = 1;
    for (const Race r : races) {
        if (rng.rand_percent() < 1 / count) {
            race = r;
        }
        count++;
    }
    return race;
}

MinionLoc Board::get_random_minion_loc(Minion* exclude) {
    MinionLoc random_loc = minions().end();
    double count = 1;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->is_zombie()) continue;
        if (&*m == exclude) continue;
        if (rng.rand_percent() < 1 / count) {
            random_loc = m;
        }
        count++;
    }
    return random_loc;
}

MinionLoc Board::get_random_minion_loc(const BitVector<Keyword>& excludeProps, Minion* exclude) {
    auto random_loc = minions().end();
    double count = 1;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->is_zombie()) continue;
        if (&*m == exclude) continue;
        if (m->props().intersects(excludeProps)) continue;
        if (rng.rand_percent() < 1 / count) {
            random_loc = m;
        }
        count++;

    }
    return random_loc;
}

MinionLoc Board::get_random_minion_loc_by_race(const Race race, Minion* exclude) {
    auto random_loc = minions().end();
    double count = 1;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->is_zombie()) continue;
        if (&*m == exclude) continue;
        if (!m->is(race)) continue;
        if (rng.rand_percent() < 1 / count) {
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

MinionLoc Board::add_minion(const Minion& minion) {
    return add_minion(minion, _minions.end());
}

MinionLoc Board::add_minion(const Minion& minion, const MinionLoc loc) {
    const MinionLoc spawn_loc = _minions.insert(loc, minion);
    proc_trigger(Keyword::ON_ADD, &*spawn_loc);
    if (minion.has(Keyword::TAUNT)) {
        _taunt_count++;
    }
    if (minion.has(Keyword::AURA)) {
        apply_aura(spawn_loc);
    }
    if (minion.has(Keyword::SPECIAL)) {
        switch (static_cast<CardDb::Id>(minion.id())) {
            case CardDb::Id::OLD_MURK_EYE:
            case CardDb::Id::OLD_MURK_EYE_G: {
                const int buff = minion.is_golden() ? 2 : 1;
                for (auto m = minions().begin(); m != minions().end(); ++m) {
                    if (m != spawn_loc && m->is(Race::MURLOC)) {
                        spawn_loc->delta_attack(buff);
                    }
                }
                break;
            }
            case CardDb::Id::POGO_HOPPER:
            case CardDb::Id::POGO_HOPPER_G: {
                _player->increment_pogo_counter();
                break;
            }
            case CardDb::Id::KHADGAR:
            case CardDb::Id::KHADGAR_G: {
                _summon_multiplier += minion.is_golden() ? 2 : 1;
                break;
            }
            default: break;
        }
    }
    register_triggers(spawn_loc);
    return spawn_loc;
}

void Board::magnetize_minion(MinionLoc minion, const Minion& other) {
    if (other.has(Keyword::TAUNT)) {
        _taunt_count++;
    }
    minion->magnetize(other);
    for (const auto& trigger: other.effects() | std::views::keys) {
        if (!minion->effects().contains(trigger)) {
            register_trigger(trigger, minion);
        }
    }
}

MinionLoc Board::play_minion(const Minion& minion) {
    return play_minion(minion, _minions.end());
}

MinionLoc Board::play_minion(const Minion& minion, MinionLoc loc) {
    if (minion.has(Keyword::MAGNETIC) && loc != minions().end() && loc->is(Race::MECHANICAL)) {
        Minion clone = minion;
        magnetize_minion(loc, clone);
        proc_trigger(Keyword::ON_PLAY, &clone);
        return loc;
    }

    if (full()) return minions().end();

    const MinionLoc spawn_loc = add_minion(minion, loc);
    if (minion.has(Keyword::BATTLECRY)) {
        exec_effects(minion.get_effects(Keyword::BATTLECRY), spawn_loc);
    }
    proc_trigger(Keyword::ON_SUMMON, &*spawn_loc);
    proc_trigger(Keyword::ON_PLAY, &*spawn_loc);
    return spawn_loc;
}

MinionLoc Board::summon_minion(const Minion& minion, const bool post_death) {
    return summon_minion(minion, _minions.end(), post_death);
}

MinionLoc Board::summon_minion(const Minion& minion, const MinionLoc loc, const bool post_death) {
    MinionLoc spawn_loc;
    for (int i = 0; i < _summon_multiplier; i++) {
        if (full(!post_death)) return minions().end();
        spawn_loc = add_minion(minion, loc);
        proc_trigger(Keyword::ON_SUMMON, &*spawn_loc);
    }
    return spawn_loc;
}

void Board::proc_enchantment(const int enchantment_id, const MinionLoc source, Minion* target) {
    Enchantment enchantment = db.get_enchantment(enchantment_id); // todo: don't copy
    switch (static_cast<CardDb::Id>(enchantment_id)) {
        case CardDb::Id::GIVE_ATTACK_E: {
            enchantment.set_attack(source->attack());
            break;
        }
        case CardDb::Id::GIVE_HEALTH_E: {
            enchantment.set_health(source->max_health());
            break;
        }
        case CardDb::Id::POGO_HOP_E:
        case CardDb::Id::POGO_HOP_G_E: {
            enchantment.set_attack((_player->pogo_counter() - 1) * enchantment.attack());
            enchantment.set_health((_player->pogo_counter() - 1) * enchantment.health());
            break;
        }
        case CardDb::Id::ANNIHILAN_MIGHT_E:
        case CardDb::Id::ANNIHILAN_MIGHT_G_E: {
            enchantment.set_health(_player->missing_health() * enchantment.health());
            break;
        }
        default: break;
    }

    switch (enchantment.target()) {
        case Target::SINGLE: {
            if (target != nullptr) {
                // apply enchantment to target
                if (enchantment.races().empty() || enchantment.races().intersects(target->races())) {
                    enchant_minion(*target, enchantment);
                }
            } else {
                // apply enchantment randomly
                if (enchantment.races().any()) {
                    const Race race = get_random_race(enchantment.races());
                    enchant_random_minion_by_race(enchantment, race);
                } else {
                    enchant_random_minion(enchantment);
                }
            }
            break;
        }
        case Target::SINGLE_OTHER: {
            if (enchantment.races().any()) {
                const Race race = get_random_race(enchantment.races());
                enchant_random_minion_by_race(enchantment, race, &*source);
            } else {
                enchant_random_minion(enchantment, &*source);
            }
            break;
        }
        case Target::ALL: {
            for (Minion& m: minions()) {
                if (enchantment.races().any() && !m.races().intersects(enchantment.races())) continue;
                enchant_minion(m, enchantment);
            }
            break;
        }
        case Target::ALL_OTHER: {
            for (auto m = minions().begin(); m != minions().end(); ++m) {
                if (m == source) continue;
                if (enchantment.races().any() && !m->races().intersects(enchantment.races())) continue;
                enchant_minion(*m, enchantment);
            }
            break;
        }
        case Target::SELF: {
            enchant_minion(*source, enchantment);
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
        case Target::ONE_OF_EACH: {
            MinionLocSet selections;
            // todo: pretty inefficient - we have to loop through all races once and all minions multiple times
            for (const Race race : enchantment.races()) {
                auto random_loc = minions().end();
                double count = 1;
                for (auto m = minions().begin(); m != minions().end(); ++m) {
                    if (m->is_zombie()) continue; // skip, is zombie
                    if (selections.contains(m)) continue; // skip, we've already selected
                    if (!m->is(race)) continue; // skip, race doesn't match
                    if (random_loc != minions().end() && m->is(Race::ALL)) continue; // skip, don't pick amalgam if we can select something else
                    if (random_loc != minions().end() && random_loc->is(Race::ALL)) {
                        random_loc = m; // replace amalgam with selection
                    }
                    if (rng.rand_percent() < 1 / count) {
                        random_loc = m;
                    }
                    count++;
                }
                enchant_minion(*random_loc, enchantment);
                selections.insert(random_loc);
            }
            break;
        }
    }
}

void Board::enchant_minion(Minion& minion, const Enchantment& enchantment, const bool aura) {
    if (enchantment.has(Keyword::TAUNT) && !minion.has(Keyword::TAUNT)) {
        _taunt_count++;
    }
    minion.props() |= enchantment.props();
    minion.delta_attack(enchantment.attack(), aura);
    minion.delta_health(enchantment.health(), aura);
}

void Board::disenchant_minion(Minion& minion, const Enchantment& enchantment, const bool aura) {
    if (enchantment.has(Keyword::TAUNT) && minion.has(Keyword::TAUNT)) {
        _taunt_count--;
    }
    minion.props() = minion.props() & ~enchantment.props();
    minion.delta_attack(-enchantment.attack(), aura);
    minion.delta_health(-enchantment.health(), aura);
}

MinionLoc Board::enchant_random_minion(const Enchantment& enchantment, Minion* exclude) {
    // todo: hold on - its only a subset of props that I'm interested in filtering out, right?
    //  - `divine shield` and `reborn` should go to units that don't have them
    //  - but what about `taunt`... that shouldn't count right?
    //  - Maybe we first try to give the buff to a minion without the prop;
    //    Then if we can't, we just give it to any minion (even if it doesn't "do anything")
    const auto minion = get_random_minion_loc(enchantment.props(), exclude);
    if (minion != minions().end()) {
        enchant_minion(*minion, enchantment);
    }
    return minion;
}

MinionLoc Board::enchant_random_minion_by_race(const Enchantment& enchantment, const Race race, Minion* exclude) {
    // todo: can we really ignore props here? answer is NO (lol)
    //  - check above todo
    const auto minion = get_random_minion_loc_by_race(race, exclude);
    if (minion != minions().end()) {
        enchant_minion(*minion, enchantment);
    }
    return minion;
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
        exec_effects(minion.get_effects(Keyword::DEATHRATTLE), loc);
    }
    if (minion.has(Keyword::REBORN)) {
        // todo: [optimize] so inefficient (maybe don't handle with effect)
        const Effect reborn_effect(Keyword::REBORN, Effect::Type::REBORN_SUMMON, {minion.id()});
        exec_effect(reborn_effect, loc);
    }
    if (minion.has(Keyword::ADJACENT_AURA)) {
        undo_adjacent_aura(loc);
    }
    if (minion.has(Keyword::AURA)) {
        undo_aura(loc);
    }
    if (minion.has(Keyword::SPECIAL)) {
        switch (static_cast<CardDb::Id>(minion.id())) {
            case CardDb::Id::KHADGAR:
            case CardDb::Id::KHADGAR_G: {
                _summon_multiplier -= loc->is_golden() ? 2 : 1;
                break;
            }
            default: break;
        }
    }
    if (minion.has(Keyword::TAUNT)) {
        _taunt_count--;
    }

    if (_active == loc) {
        increment_active();
    }
    deregister_triggers(loc);
    proc_trigger(Keyword::ON_DEATH_OTHER, &*loc);

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
            exec_effects(minion.get_effects(Keyword::ON_DAMAGE_SELF), loc);
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

void Board::exec_effects(const std::vector<Effect>& effects, MinionLoc source, Minion* target) {
    for (const Effect& effect : effects) {
        exec_effect(effect, source, target);
    }
}

void Board::exec_effect(const Effect& effect, const MinionLoc source, Minion* target) {
    switch (effect.type()) {
        case Effect::Type::SUMMON: {
            for (const int minion_id: effect.args()) {
                const bool post_death = effect.trigger() == Keyword::DEATHRATTLE;
                summon_minion(db.get_minion(minion_id), get_right_minion_loc(source), post_death);
            }
            break;
        }
        case Effect::Type::SUMMON_SPECIAL: {
            for (const int arg: effect.args()) {
                switch (static_cast<Effect::SpecialSummon>(arg)) {
                    case Effect::SpecialSummon::FIRST_TWO_DEAD_MECHS: {
                        // todo
                        break;
                    }
                    case Effect::SpecialSummon::RANDOM_TIER_1:
                    case Effect::SpecialSummon::RANDOM_TIER_2:
                    case Effect::SpecialSummon::RANDOM_TIER_3:
                    case Effect::SpecialSummon::RANDOM_TIER_4:
                    case Effect::SpecialSummon::RANDOM_TIER_5:
                    case Effect::SpecialSummon::RANDOM_TIER_6:
                    case Effect::SpecialSummon::RANDOM_TIER_7: {
                        const Minion& minion = db.get_minion(_player->pool()->get_random_minionid_from_tier(arg));
                        const bool post_death = effect.trigger() == Keyword::DEATHRATTLE;
                        summon_minion(minion, get_right_minion_loc(source), post_death);
                        break;
                    }
                    case Effect::SpecialSummon::RANDOM_DEATHRATTLE: {
                        // todo
                        break;
                    }
                    case Effect::SpecialSummon::RANDOM_LEGENDARY: {
                        // todo
                        break;
                    }
                    case Effect::SpecialSummon::RAT_PACK: {
                        const Minion& rat = db.get_minion(CardDb::Id::RAT_T);
                        for (int j = 0; j < source->attack(); j++) {
                            summon_minion(rat, get_right_minion_loc(source), true);
                        }
                        break;
                    }
                    case Effect::SpecialSummon::RAT_PACK_GOLDEN: {
                        const Minion& rat = db.get_minion(CardDb::Id::RAT_T_G);
                        for (int j = 0; j < source->attack(); j++) {
                            summon_minion(rat, get_right_minion_loc(source), true);
                        }
                        break;
                    }
                    case Effect::SpecialSummon::THE_BEAST: {
                        const Minion& pip = db.get_minion(CardDb::Id::PIP_QUICKWIT);
                        for (int i = 0; i < _summon_multiplier; i++) {
                            _player->opponent()->board().summon_minion(pip);
                        }
                        break;
                    }
                }
            }
            break;
        }
        case Effect::Type::REBORN_SUMMON: {
            const int minion_id = effect.args()[0];
            Minion minion = db.get_minion(minion_id);
            minion.set_health(1);
            minion.clear(Keyword::REBORN);
            summon_minion(minion, get_right_minion_loc(source), true);
            break;
        }
        case Effect::Type::ENCHANT: {
            for (const int enchantment_id: effect.args()) {
                proc_enchantment(enchantment_id, source, target);
            }
            break;
        }
        case Effect::Type::GEN_CARD: {
            // todo
            break;
        }
        case Effect::Type::TRIGGER_ADJACENT_BATTLECRY: {
            const auto l = get_left_minion_loc(source);
            const auto r = get_right_minion_loc(source);
            const bool is_battlecry_left = is_minion(l) && l->has(Keyword::BATTLECRY);
            const bool is_battlecry_right = is_minion(r) && r->has(Keyword::BATTLECRY);
            if (is_battlecry_left && is_battlecry_right) {
                if (rng.coin_flip()) {
                    exec_effects(l->get_effects(Keyword::BATTLECRY), l);
                } else {
                    exec_effects(r->get_effects(Keyword::BATTLECRY), r);
                }
            } else if (is_battlecry_left) {
                exec_effects(l->get_effects(Keyword::BATTLECRY), l);
            } else if (is_battlecry_right) {
                exec_effects(r->get_effects(Keyword::BATTLECRY), r);
            }
            break;
        }
        case Effect::Type::TRIGGER_ADJACENT_BATTLECRIES: {
            const auto l = get_left_minion_loc(source);
            if (is_minion(l) && l->has(Keyword::BATTLECRY)) {
                exec_effects(l->get_effects(Keyword::BATTLECRY), l);
            }
            const auto r = get_right_minion_loc(source);
            if (is_minion(r) && r->has(Keyword::BATTLECRY)) {
                exec_effects(r->get_effects(Keyword::BATTLECRY), r);
            }
            break;
        }
        case Effect::Type::DEAL_DAMAGE_PLAYER: {
            for (const int damage : effect.args()) {
                _player->deal_damage(damage);
                proc_trigger(Keyword::ON_DAMAGE_PLAYER);
            }
            break;
        }
        case Effect::Type::DEAL_DAMAGE_OTHER: {
            Board& opp_board = _player->opponent()->board();
            MinionLocSet zombies;
            for (const int damage : effect.args()) {
                const MinionLoc opp_loc = opp_board.get_random_minion_loc();
                if (opp_loc != opp_board.minions().end()) {
                    opp_board.damage_minion(opp_loc, damage);
                    if (opp_loc->is_zombie()) {
                        zombies.insert(opp_loc);
                    }
                }
            }
            for (const MinionLoc minion_loc : zombies) {
                opp_board.reap_minion(minion_loc);
            }
            break;
        }
    }
}

void Board::apply_adjacent_aura(const MinionLoc loc) {
    const std::vector<Effect>& effects = loc->get_effects(Keyword::ADJACENT_AURA);
    for (const Effect& effect : effects) {
        for (const int enchantment_id: effect.args()) {
            const Enchantment& enchantment = db.get_enchantment(enchantment_id);
            const MinionLoc left = get_left_minion_loc(loc);
            if (left != minions().end()) {
                enchant_minion(*left, enchantment, true);
                loc->set_left_adjacent(&*left);
            }
            MinionLoc right = get_right_minion_loc(loc);
            if (right != minions().end()) {
                enchant_minion(*right, enchantment, true);
                loc->set_right_adjacent(&*right);
            }
        }
    }
}

void Board::undo_adjacent_aura(const MinionLoc loc) {
    Minion* left_adjacent = loc->left_adjacent();
    Minion* right_adjacent = loc->right_adjacent();
    const std::vector<Effect>& effects = loc->get_effects(Keyword::ADJACENT_AURA);
    for (const Effect& effect : effects) {
        for (const int enchantment_id: effect.args()) {
            const Enchantment& enchantment = db.get_enchantment(enchantment_id);
            if (left_adjacent != nullptr && !left_adjacent->is_zombie()) {
                disenchant_minion(*left_adjacent, enchantment, true);
                loc->set_left_adjacent(nullptr);
            }
            if (right_adjacent != nullptr && !right_adjacent->is_zombie()) {
                disenchant_minion(*right_adjacent, enchantment, true);
                loc->set_right_adjacent(nullptr);
            }
        }
    }
}

void Board::apply_aura(const MinionLoc loc) {
    const std::vector<Effect>& effects = loc->get_effects(Keyword::AURA);
    for (const Effect& effect : effects) {
        for (const int enchantment_id: effect.args()) {
            Enchantment enchantment = db.get_enchantment(enchantment_id);
            for (auto m = minions().begin(); m != minions().end(); ++m) {
                if (enchantment.races().any() && !m->races().intersects(enchantment.races())) continue;
                if (enchantment.constraints().any() && !m->props().intersects(enchantment.constraints())) continue;
                if (enchantment.target() == Target::ALL) {
                    enchant_minion(*m, enchantment);
                } else if (enchantment.target() == Target::ALL_OTHER && m != loc) {
                    enchant_minion(*m, enchantment);
                }
            }
        }
    }
}

void Board::undo_aura(const MinionLoc loc) {
    const std::vector<Effect>& effects = loc->get_effects(Keyword::AURA);
    for (const Effect& effect : effects) {
        for (const int enchantment_id: effect.args()) {
            Enchantment enchantment = db.get_enchantment(enchantment_id);
            for (auto m = minions().begin(); m != minions().end(); ++m) {
                if (enchantment.races().any() && !m->races().intersects(enchantment.races())) continue;
                // todo: [BUG] handle case where we disenchant taunt, so we need to unapply the aura for "Phalanx Commander"
                if (enchantment.constraints().any() && !m->props().intersects(enchantment.constraints())) continue;
                if (enchantment.target() == Target::ALL) {
                    disenchant_minion(*m, enchantment);
                } else if (enchantment.target() == Target::ALL_OTHER && m != loc) {
                    disenchant_minion(*m, enchantment);
                }
            }
        }
    }
}

void Board::turn_start() {
    proc_trigger(Keyword::ON_TURN_START);
}

void Board::turn_end() {
    proc_trigger(Keyword::ON_TURN_END);
}

void Board::pre_combat() {
    proc_trigger(Keyword::ON_PRE_COMBAT);
}

void Board::post_combat() {
    proc_trigger(Keyword::ON_POST_COMBAT);
}

void Board::pre_battle() {
    _active = _minions.begin();
    // todo: when we clone the list, all of our iterators are invalidated - they point to the original list.
    //  - so we have to recompute them.
    //  - is there anything clever we can do about this?
    //  - we coid: override the copy constructor to do this
    _triggers.clear();
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        register_triggers(m);
    }
}

void Board::increment_active() {
    if (empty(true)) return;

    ++_active;
    if (_active == _minions.end()) {
        _active = _minions.begin(); // wraparound
    }
}

void Board::proc_trigger(const Keyword trigger, Minion* source) {
    if (!_triggers.contains(trigger)) return;

    for (const MinionLoc listener : _triggers.at(trigger)) { // todo: remove array indexing (inefficient)
        if (listener->has(Keyword::SPECIAL)) {
            switch (static_cast<CardDb::Id>(listener->id())) {
                case CardDb::Id::OLD_MURK_EYE:
                case CardDb::Id::OLD_MURK_EYE_G: {
                    if (source->is(Race::MURLOC)) {
                        int buff = listener->is_golden() ? 2 : 1;
                        if (trigger == Keyword::ON_DEATH_OTHER) {
                            buff = -buff;
                        }
                        listener->delta_attack(buff);
                    }
                    break;
                }
                default: break;
            }
        } else if (listener->has(Keyword::ADJACENT_AURA)) {
            if (trigger == Keyword::ON_PRE_COMBAT) {
                apply_adjacent_aura(listener);
            } else if (trigger == Keyword::ON_POST_COMBAT) {
                undo_adjacent_aura(listener);
            }
        } else if (listener->has(Keyword::AURA)) {
            if (trigger == Keyword::ON_ADD) {
                const std::vector<Effect>& effects = listener->get_effects(Keyword::AURA);
                for (const Effect& effect : effects) {
                    for (const int enchantment_id: effect.args()) {
                        const Enchantment& enchantment = db.get_enchantment(enchantment_id);
                        enchant_minion(*source, enchantment, true);
                    }
                }
            }
        } else {
            const std::vector<Effect>& effects = listener->get_effects(trigger);
            for (const Effect& effect : effects) {
                if (effect.constraint() == Effect::Constraint::NONE) {
                    exec_effect(effect, listener, source);
                } else if (source != nullptr && source->satisfiesConstraint(effect.constraint())) {
                    exec_effect(effect, listener, source);
                }
            }
        }
    }
}

void Board::register_trigger(const Keyword trigger, const MinionLoc loc) {
    _triggers[trigger].insert(loc);
}

void Board::deregister_trigger(const Keyword trigger, const MinionLoc loc) {
    _triggers[trigger].erase(loc);
    if (_triggers[trigger].empty()) {
        _triggers.erase(trigger);
    }
}

void Board::register_triggers(const MinionLoc loc) {
    if (loc->has(Keyword::ADJACENT_AURA)) {
        register_trigger(Keyword::ON_PRE_COMBAT, loc);
        register_trigger(Keyword::ON_POST_COMBAT, loc);
    }
    if (loc->has(Keyword::AURA)) {
        register_trigger(Keyword::ON_ADD, loc);
    }
    if (loc->has(Keyword::SPECIAL)) {
        switch (static_cast<CardDb::Id>(loc->id())) {
            case CardDb::Id::OLD_MURK_EYE:
            case CardDb::Id::OLD_MURK_EYE_G: {
                register_trigger(Keyword::ON_ADD, loc);
                register_trigger(Keyword::ON_DEATH_OTHER, loc);
                break;
            }
            default: break;
        }
    }
    for (const auto& keyword: loc->effects() | std::views::keys) {
        if (KeywordUtil::isTrigger(keyword)) {
            register_trigger(keyword, loc);
        }
    }
}

void Board::deregister_triggers(const MinionLoc loc) {
    if (loc->has(Keyword::ADJACENT_AURA)) {
        deregister_trigger(Keyword::ON_PRE_COMBAT, loc);
        deregister_trigger(Keyword::ON_POST_COMBAT, loc);
    }
    if (loc->has(Keyword::AURA)) {
        deregister_trigger(Keyword::ON_SUMMON, loc);
    }
    if (loc->has(Keyword::SPECIAL)) {
        switch (static_cast<CardDb::Id>(loc->id())) {
            case CardDb::Id::OLD_MURK_EYE:
            case CardDb::Id::OLD_MURK_EYE_G: {
                deregister_trigger(Keyword::ON_ADD, loc);
                deregister_trigger(Keyword::ON_DEATH_OTHER, loc);
                break;
            }
            default: break;
        }
    }
    for (const auto& keyword: loc->effects() | std::views::keys) {
        if (KeywordUtil::isTrigger(keyword)) {
            deregister_trigger(keyword, loc);
        }
    }
}

std::unordered_map<Keyword, MinionLocSet> Board::triggers() const {
    return _triggers;
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
    return include_zombies ? _minions.size() : _minions.size() - _zombie_count;
}

bool Board::empty(const bool include_zombies) const {
    return size(include_zombies) == 0;
}

bool Board::full(const bool include_zombies) const {
    return size(include_zombies) >= 7;
}

int Board::taunt_count() const {
    return _taunt_count;
}

int Board::zombie_count() const {
    return _zombie_count;
}

void Board::bind_player(Player* player) {
    _player = player;
}

[[nodiscard]] std::string Board::to_string() {
    std::ostringstream oss;
    oss << _minions.size() << " | ";
    for (auto& minion: _minions) {
        oss << minion << " | ";
    }
    return oss.str();
}
