#include "Effect.h"

Effect::Effect(const json& json) {
    _trigger = KeywordUtil::fromString(json["trigger"]);
    _type = TypeUtil::fromString(json["type"]);
    for (const auto& arg_json : json["args"]) {
        if (arg_json.is_number_integer()) {
            _args.push_back(arg_json);
        }
    }
}

Effect::Effect(const Keyword trigger, const Type type, const std::vector<int>& args) {
    _trigger = trigger;
    _type = type;
    _args = args;
}
