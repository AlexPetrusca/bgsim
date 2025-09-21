#include "Effect.h"

Effect::Type Effect::TypeUtil::fromString(const std::string& str) {
    return type_map.find(str)->second;
}

Effect::Constraint Effect::ConstraintUtil::fromString(const std::string& str) {
    const auto pair = constraint_map.find(str);
    return pair != constraint_map.end() ? pair->second : Constraint::NONE;
}

bool Effect::ConstraintUtil::matchesRace(const Constraint constraint, const BitVector<Race>& races) {
    // todo: can we us this with some clever logic instead?
    //  - Maybe you can just use bit manipulation
    switch (constraint) {
        case Constraint::NONE:
            return true;
        case Constraint::BEAST_ONLY:
            return races.has(Race::BEAST);
        case Constraint::DEMON_ONLY:
            return races.has(Race::DEMON);
        case Constraint::DRAGON_ONLY:
            return races.has(Race::DRAGON);
        case Constraint::ELEMENTAL_ONLY:
            return races.has(Race::ELEMENTAL);
        case Constraint::MECHANICAL_ONLY:
            return races.has(Race::MECHANICAL);
        case Constraint::MURLOC_ONLY:
            return races.has(Race::MURLOC);
        case Constraint::NAGA_ONLY:
            return races.has(Race::NAGA);
        case Constraint::PIRATE_ONLY:
            return races.has(Race::PIRATE);
        case Constraint::QUILLBOAR_ONLY:
            return races.has(Race::QUILLBOAR);
        case Constraint::UNDEAD_ONLY:
            return races.has(Race::UNDEAD);
    }
    return false;
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

const std::vector<int>& Effect::args() const {
    return _args;
}
