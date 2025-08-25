#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

#include "../../IPrintable.h"

class Minion : public IPrintable {
public:
    Minion(std::string name, int tier, int attack, int health);
    
    [[nodiscard]] std::string to_string() override;

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

private:
    std::string _name;
    int _tier;
    int _attack;
    int _health;
    // vector<buff_t> buffs;
    // vector<buff_t> tempBuffs;
    // vector<buff_t> auraBuffs;
    // vector<effect_t> effects;
};

#endif //CARD_H
