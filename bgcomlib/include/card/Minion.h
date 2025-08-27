#ifndef CARD_H
#define CARD_H

#include <string>
#include <unordered_set>
#include <vector>

#include "Effect.h"
#include "Keyword.h"
#include "../../IPrintable.h"

class Minion : public IPrintable {
public:
    Minion();

    Minion(const json& minion_json);

    Minion(std::string name, int tier, int attack, int health); // todo: delete me

    [[nodiscard]] std::string name() const {
        return _name;
    }

    void set_name(const std::string &name) {
        _name = name;
    }

    [[nodiscard]] int tier() const {
        return _tier;
    }

    void set_tier(const int tier) {
        _tier = tier;
    }

    [[nodiscard]] int attack() const {
        return _attack;
    }

    void set_attack(const int attack) {
        _attack = attack;
    }

    [[nodiscard]] int health() const {
        return _health;
    }

    void set_health(const int health) {
        _health = health;
    }

    int deal_damage(const int damage) {
        _health -= damage;
        return _health;
    }

    [[nodiscard]] bool has_keyword(const Keyword keyword) const {
        return _keywords.contains(keyword);
    }

    [[nodiscard]] const Effect& get_effect(const Keyword keyword) const {
        return _effects.at(keyword);
    }

    [[nodiscard]] int id() const {
        return _id;
    }

    [[nodiscard]] int alt_id() const {
        return _alt_id;
    }

    [[nodiscard]] bool is_golden() const {
        return _is_golden;
    }

    [[nodiscard]] bool is_divine_shield() const {
        return _is_divine_shield;
    }

    void set_divine_shield(bool is_divine_shield) {
        _is_divine_shield = is_divine_shield;
    }

    [[nodiscard]] bool is_taunt() const {
        return _is_taunt;
    }

    void set_taunt(bool is_taunt) {
        _is_taunt = is_taunt;
    }

    [[nodiscard]] bool is_reborn() const {
        return _is_reborn;
    }

    void set_reborn(bool is_reborn) {
        _is_reborn = is_reborn;
    }

    [[nodiscard]] bool is_deathrattle() const {
        return _is_deathrattle;
    }

    [[nodiscard]] std::string to_string() override;

private:
    std::string _name;
    int _tier{};
    std::unordered_set<Keyword> _keywords;
    std::unordered_map<Keyword, Effect> _effects;

    int _id{};
    int _alt_id{};
    bool _is_golden{};

    int _attack{};
    int _health{};
    bool _is_divine_shield{};
    bool _is_taunt{};
    bool _is_reborn{};
    bool _is_deathrattle{};

    // vector<buff_t> buffs;
    // vector<buff_t> tempBuffs;
    // vector<buff_t> auraBuffs;
    // vector<effect_t> effects;
};

#endif //CARD_H
