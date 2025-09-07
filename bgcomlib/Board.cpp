#include "include/Board.h"

#include <random>
#include <sstream>

#include "card/CardDb.h"

Board::Board(const std::vector<Minion>& minions) {
    if (minions.size() > 7) {
        throw std::invalid_argument("Too many minions: " + std::to_string(minions.size()));
    }
    this->_minions = std::list(minions.begin(), minions.end());
    this->_zombie_count = 0;
    this->_taunt_count = 0;
    for (const Minion& minion: _minions) {
        if (minion.has(Keyword::TAUNT)) {
            _taunt_count++;
        }
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
    double count = 0;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        count++;
        if (dist(_rng) < 1 / count) {
            random_loc = m;
        }
    }
    return random_loc;
}

MinionLoc Board::get_random_minion_loc(const BitVector<Keyword>& exclude) {
    auto random_loc = minions().end();
    std::uniform_real_distribution<> dist(0, 1);
    double count = 0;
    for (auto m = minions().begin(); m != minions().end(); ++m) {
        if (m->props() & exclude) continue;
        count++;
        if (dist(_rng) < 1 / count) {
            random_loc = m;
        }
    }
    return random_loc;
}

void Board::summon_minion(const Minion& minion, const bool post_death) {
    summon_minion(minion, _minions.end(), post_death);
}

void Board::summon_minion(const Minion& minion, const MinionLoc loc, const bool post_death) {
    if (full(!post_death)) return;

    _minions.insert(loc, minion);

    if (minion.has(Keyword::TAUNT)) {
        _taunt_count++;
    }
}

void Board::enchant_minion(Minion& minion, const Enchantment& enchantment) {
    minion.props() |= enchantment.props();
}

void Board::enchant_random_minion(const Enchantment& enchantment) {
    const auto minion = get_random_minion_loc(enchantment.props());
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
                Enchantment enchantment = db.get_enchantment(enchantment_id);
                switch (enchantment.target()) {
                    case Target::SINGLE: {
                        enchant_random_minion(enchantment);
                        break;
                    }
                    case Target::ALL: {
                        for (auto & m : minions()) {
                            enchant_minion(m, enchantment);
                        }
                        break;
                    }
                    case Target::LEFTMOST: {
                        enchant_minion(minions().front(), enchantment);
                        break;
                    }
                    case Target::RIGHTMOST: {
                        enchant_minion(minions().back(), enchantment);
                        break;
                    }
                    default: ;
                }
            }
            break;
        }
        default:
            break;
    }
}

void Board::prep_for_battle() {
    _active = _minions.begin();
}

void Board::increment_active() {
    if (empty(true)) return;

    ++_active;
    if (_active == _minions.end()) {
        _active = _minions.begin(); // wraparound
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
