#include "include/card/Enchantment.h"

Enchantment::Enchantment() = default;

Enchantment::Enchantment(const json& enchant_json) {
    _id = enchant_json["dbfId"];
    _target = TargetUtil::fromString(enchant_json["target"]);
    if (enchant_json.contains("mechanics")) {
        for (const auto& keyword_json : enchant_json["mechanics"]) {
            _props.set(KeywordUtil::fromString(keyword_json));
        }
    }
    if (enchant_json.contains("attack")) {
        _attack = enchant_json["attack"];
    }
    if (enchant_json.contains("health")) {
        _health = enchant_json["health"];
    }
}
