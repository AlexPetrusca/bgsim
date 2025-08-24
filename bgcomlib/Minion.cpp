#include "include/Minion.h"

#include <sstream>
#include <utility>

Minion::Minion(std::string name, int32_t tier, int32_t attack, int32_t health) {
    this->_name = std::move(name);
    this->_tier = tier;
    this->_attack = attack;
    this->_health = health;
}

std::string Minion::to_string() const {
    std::ostringstream oss;
    oss << "\"" << _name << "\" (" << _attack << "/" << _health << ")";
    return oss.str();
}
