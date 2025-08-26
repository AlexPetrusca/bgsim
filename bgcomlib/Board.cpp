#include "include/Board.h"

#include <sstream>

#include "card/CardDb.h"

Board::Board(const std::vector<Minion> &minions) {
    if (minions.size() > 7) {
        throw std::invalid_argument("Too many minions: " + std::to_string(minions.size()));
    }
    this->minions = minions;
}

std::vector<Minion>& Board::get_minions() {
    return this->minions;
}

void Board::add_minion(const Minion &minion, const size_t idx) {
    if (full()) return;

    if (idx == -1) {
        minions.push_back(minion);
    } else {
        minions.insert(minions.begin() + idx, minion);
    }
}

void Board::kill_minion(size_t idx) {
    Minion minion = minions.at(idx);
    minions.erase(minions.begin() + idx);
    if (minion.has_keyword(Keyword::DEATHRATTLE)) {
        exec_effect(minion.get_effect(Keyword::DEATHRATTLE), idx);
    }
}

void Board::exec_effect(const Effect& effect, const size_t idx) {
    if (effect.type() == Effect::Type::SUMMON) {
        for (int minion_id : effect.args()) {
            if (full()) break;
            add_minion(db.get_minion(minion_id), idx);
        }
    }
}

int Board::tier_total() const {
    int total = 0;
    for (const Minion &minion : minions) {
        total += minion.tier();
    }
    return total;
}

size_t Board::size() const {
    return minions.size();
}

bool Board::empty() const {
    return minions.empty();
}

bool Board::full() const {
    return minions.size() == 7;
}

[[nodiscard]] std::string Board::to_string() {
    std::ostringstream oss;
    oss << minions.size() << " | ";
    for (auto& minion : minions) {
        oss << minion << " | ";
    }
    return oss.str();
}
