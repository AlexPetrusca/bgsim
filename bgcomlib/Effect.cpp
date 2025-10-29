#include "Effect.h"

Effect::Type Effect::TypeUtil::fromString(const std::string& str) {
    return type_map.find(str)->second;
}

Effect::Constraint Effect::ConstraintUtil::fromString(const std::string& str) {
    const auto pair = constraint_map.find(str);
    return pair != constraint_map.end() ? pair->second : Constraint::NONE;
}

Race Effect::DiscoverUtil::to_race(Discover discover) {
    switch (discover) {
        case Discover::BEAST:
            return Race::BEAST;
        case Discover::DEMON:
            return Race::DEMON;
        case Discover::DRAGON:
            return Race::DRAGON;
        case Discover::ELEMENTAL:
            return Race::ELEMENTAL;
        case Discover::MECHANICAL:
            return Race::MECHANICAL;
        case Discover::MURLOC:
            return Race::MURLOC;
        case Discover::NAGA:
            return Race::NAGA;
        case Discover::PIRATE:
            return Race::PIRATE;
        case Discover::QUILLBOAR:
            return Race::QUILLBOAR;
        case Discover::UNDEAD:
            return Race::UNDEAD;
        default:
            std::unreachable();
    }
}

Keyword Effect::DiscoverUtil::to_keyword(Discover discover) {
    switch (discover) {
        case Discover::BATTLECRY:
            return Keyword::BATTLECRY;
        case Discover::DEATHRATTLE:
            return Keyword::DEATHRATTLE;
        default:
            std::unreachable();
    }
}

Effect::Effect(const json& json) {
    _trigger = KeywordUtil::fromString(json["trigger"]);
    _type = TypeUtil::fromString(json["type"]);
    if (json.contains("constraint")) {
        _constraint = ConstraintUtil::fromString(json["constraint"]);
    } else {
        _constraint = Constraint::NONE;
    }
    if (json.contains("args")) {
        for (const auto& arg_json : json["args"]) {
            if (arg_json.is_number_integer()) {
                _args.push_back(arg_json);
            }
        }
    }
}

Effect::Effect(const Keyword trigger, const Type type, const std::vector<int>& args) {
    _trigger = trigger;
    _type = type;
    _args = args;
    _constraint = Constraint::NONE; // todo: do we need to add this to the constructor?
}

Keyword Effect::trigger() const {
    return _trigger;
}

Effect::Constraint Effect::constraint() const {
    return _constraint;
}

Effect::Type Effect::type() const {
    return _type;
}

std::vector<int>& Effect::args() const {
    return _args;
}
