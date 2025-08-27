#include "include/Board.h"

#include <sstream>

#include "card/CardDb.h"

Board::Board(const std::vector<Minion> &minions) {
    if (minions.size() > 7) {
        throw std::invalid_argument("Too many minions: " + std::to_string(minions.size()));
    }
    this->_minions = minions;
    this->_taunt_count = 0;
    for (const Minion& minion : _minions) {
        if (minion.is_taunt()) {
            _taunt_count++;
        }
    }
}

Board Board::from_ids(const std::vector<CardDb::Id>& minionIds) {
    if (minionIds.size() > 7) {
        throw std::invalid_argument("Too many minions: " + std::to_string(minionIds.size()));
    }
    std::vector<Minion> minions;
    for (const CardDb::Id& minionId : minionIds) {
        minions.push_back(db.get_minion(minionId));
    }
    return Board(minions);
}

std::vector<Minion>& Board::get_minions() {
    return this->_minions;
}

void Board::add_minion(const Minion &minion, const size_t idx) {
    if (full()) return;

    if (idx == -1) {
        _minions.push_back(minion);
    } else {
        _minions.insert(_minions.begin() + idx, minion);
    }

    if (minion.is_taunt()) {
        _taunt_count++;
    }
}

void Board::kill_minion(size_t idx) {
    Minion minion = _minions.at(idx);
    _minions.erase(_minions.begin() + idx);
    if (minion.is_deathrattle()) {
        exec_effect(minion.get_effect(Keyword::DEATHRATTLE), idx);
    }
    if (minion.is_reborn()) {
        // todo: [optimize] so inefficient (maybe don't handle with effect)
        const Effect reborn_effect(Keyword::REBORN, Effect::Type::REBORN_SUMMON, {minion.id()});
        exec_effect(reborn_effect, idx);
    }
    if (minion.is_taunt()) {
        _taunt_count--;
    }
}

void Board::exec_effect(const Effect& effect, const size_t idx) {
    switch (effect.type()) {
        case Effect::Type::SUMMON: {
            for (const int minion_id : effect.args()) {
                add_minion(db.get_minion(minion_id), idx);
            }
            break;
        }
        case Effect::Type::REBORN_SUMMON: {
            const int minion_id = effect.args()[0];
            Minion minion = db.get_minion(minion_id);
            minion.set_health(1);
            minion.set_reborn(false);
            add_minion(minion, idx);
            break;
        }
        default:
            break;
    }
}

int Board::tier_total() const {
    int total = 0;
    for (const Minion &minion : _minions) {
        total += minion.tier();
    }
    return total;
}

size_t Board::size() const {
    return _minions.size();
}

bool Board::empty() const {
    return _minions.empty();
}

bool Board::full() const {
    return _minions.size() == 7;
}

int Board::taunt_count() const {
    return _taunt_count;
}

[[nodiscard]] std::string Board::to_string() {
    std::ostringstream oss;
    oss << _minions.size() << " | ";
    for (auto& minion : _minions) {
        oss << minion << " | ";
    }
    return oss.str();
}
