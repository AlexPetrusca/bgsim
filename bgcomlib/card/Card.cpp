#include "card/Card.h"

#include <sstream>

int Card::id() const {
    return _id;
}

std::string Card::name() const {
    return _name;
}

void Card::set_name(const std::string& name) {
    this->_name = name;
}

std::string Card::to_string() {
    std::ostringstream oss;
    oss << _name << " (" << _id << ")";
    return oss.str();
}
