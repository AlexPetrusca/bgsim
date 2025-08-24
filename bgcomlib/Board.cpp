#include "include/Board.h"

#include <sstream>

Board::Board(const std::vector<Minion> &minions) {
    this->minions = minions;
}

std::vector<Minion> Board::get_minions() {
    return this->minions;
}

void Board::add_minion(const Minion &minion, const int idx = -1) {
    if (idx == -1) {
        minions.push_back(minion);
    } else {
        minions.insert(minions.begin() + idx, minion);
    }
}

[[nodiscard]] std::string Board::to_string() const {
    std::ostringstream oss;
    for (const auto& minion : minions) {
        oss << minion << " ";
    }
    return oss.str();
}
