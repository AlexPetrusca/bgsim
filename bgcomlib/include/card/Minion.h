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

    [[nodiscard]] std::string name() const;

    void set_name(const std::string &name);

    [[nodiscard]] int tier() const;

    void set_tier(int tier);

    [[nodiscard]] int attack() const;

    void set_attack(int attack);

    [[nodiscard]] int health() const;

    void set_health(int health);

    int deal_damage(int damage);

    int delta_attack(int delta);

    int delta_health(int delta);

    [[nodiscard]] const Effect& get_effect(Keyword keyword) const;

    [[nodiscard]] int id() const;

    [[nodiscard]] int alt_id() const;

    [[nodiscard]] bool is_golden() const;

    [[nodiscard]] bool is_zombie() const;

    void set_zombie(bool is_zombie);

    [[nodiscard]] bool is_poisoned() const;

    void set_poisoned(bool is_poisoned);

    BitVector<Keyword>& props();

    [[nodiscard]] bool has(Keyword keyword) const;

    void set(Keyword keyword);

    void clear(Keyword keyword);

    void toggle(Keyword keyword);

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
