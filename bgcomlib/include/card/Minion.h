#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

#include "Effect.h"
#include "Keyword.h"
#include "../../IPrintable.h"

class Minion : public IPrintable {
public:
    Minion();

    explicit Minion(const json& minion_json);

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

    [[nodiscard]] bool is_zombie() const {
        return _is_zombie;
    }

    void set_zombie(const bool is_zombie) {
        _is_zombie = is_zombie;
    }

    [[nodiscard]] bool is_poisoned() const {
        return _is_poisoned;
    }

    void set_poisoned(const bool is_poisoned) {
        _is_poisoned = is_poisoned;
    }

    BitVector<Keyword>& props() {
        return _props;
    }

    [[nodiscard]] bool has(const Keyword keyword) const {
        return _props.has(keyword);
    }

    void set(const Keyword keyword) {
        return _props.set(keyword);
    }

    void clear(const Keyword keyword) {
        return _props.clear(keyword);
    }

    void toggle(const Keyword keyword) {
        if (_props.has(keyword)) {
            _props.clear(keyword);
        } else {
            _props.set(keyword);
        }
    }

    [[nodiscard]] std::string to_string() override;

private:
    // todo: how many of these fields can you get rid of
    std::string _name;
    int _tier{};
    std::unordered_map<Keyword, Effect> _effects;

    int _id{};
    int _alt_id{};
    bool _is_golden{};

    int _attack{};
    int _health{};
    BitVector<Keyword> _props;
    bool _is_zombie{}; // todo: this can be a prop?
    bool _is_poisoned{}; // todo: this can be a prop?

    // vector<buff_t> buffs;
    // vector<buff_t> tempBuffs;
    // vector<buff_t> auraBuffs;
    // vector<effect_t> effects;
};

#endif //CARD_H
