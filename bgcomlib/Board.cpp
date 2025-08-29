#include "include/Board.h"

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

void Board::summon_minion(const Minion& minion) {
    summon_minion(minion, _minions.end());
}

void Board::summon_minion(const Minion& minion, const MinionLoc loc) {
    if (full()) return;

    _minions.insert(loc, minion);

    if (minion.has(Keyword::TAUNT)) {
        _taunt_count++;
    }
}

void Board::kill_minion(const MinionLoc loc) {
    _zombie_count++;

    const Minion& minion = *loc;
    const MinionLoc nextLoc = std::next(loc);
    if (minion.has(Keyword::DEATHRATTLE)) {
        exec_effect(minion.get_effect(Keyword::DEATHRATTLE), nextLoc);
    }
    if (minion.has(Keyword::REBORN)) {
        // todo: [optimize] so inefficient (maybe don't handle with effect)
        const Effect reborn_effect(Keyword::REBORN, Effect::Type::REBORN_SUMMON, {minion.id()});
        exec_effect(reborn_effect, nextLoc);
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

bool Board::damage_minion(const MinionLoc loc, const int damage) {
    Minion& minion = *loc;
    const MinionLoc nextLoc = std::next(loc);

    // resolve damage
    if (minion.has(Keyword::DIVINE_SHIELD)) {
        minion.clear(Keyword::DIVINE_SHIELD);
    } else {
        minion.deal_damage(damage);
        if (minion.has(Keyword::ON_DAMAGE_SELF)) {
            exec_effect(minion.get_effect(Keyword::ON_DAMAGE_SELF), nextLoc);
        }
    }

    // resolve death
    if (minion.health() <= 0) {
        kill_minion(loc);
        return true;
    }
    return false;
}

// todo: do you want loc to be "from the left" or "from the right"
//      - right now you're passing "from the right" (i.e. nextLoc)
void Board::exec_effect(const Effect& effect, const MinionLoc loc) {
    switch (effect.type()) {
        case Effect::Type::SUMMON: {
            for (const int minion_id: effect.args()) {
                summon_minion(db.get_minion(minion_id), loc);
            }
            break;
        }
        case Effect::Type::REBORN_SUMMON: {
            const int minion_id = effect.args()[0];
            Minion minion = db.get_minion(minion_id);
            minion.set_health(1);
            minion.clear(Keyword::REBORN);
            summon_minion(minion, loc);
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
    if (empty()) return;

    ++_active;
    if (_active == _minions.end()) {
        _active = _minions.begin(); // wraparound
    }

    // // todo: we only need this for effects like cleave - uncomment later
    // while (_active->health() < 0) { // todo: replace with something like "->dead()"
    //     ++_active;
    //     if (_active == _minions.end()) {
    //         _active = _minions.begin(); // wraparound
    //     }
    // }
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

size_t Board::size() const {
    return _minions.size() - _zombie_count;
}

bool Board::empty() const {
    return size() == 0;
}

bool Board::full() const {
    return size() == 7;
}

int Board::taunt_count() const {
    return _taunt_count;
}

[[nodiscard]] std::string Board::to_string() {
    std::ostringstream oss;
    oss << _minions.size() << " | ";
    for (auto& minion: _minions) {
        oss << minion << " | ";
    }
    return oss.str();
}
