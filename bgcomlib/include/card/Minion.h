#ifndef CARD_H
#define CARD_H

#include <string>

#include "Effect.h"
#include "Keyword.h"
#include "Race.h"
#include "../../IPrintable.h"

class Minion : public IPrintable {
public:
    Minion();

    explicit Minion(const json& json);

    Minion(std::string name, int tier, int attack, int health); // todo: delete me

    [[nodiscard]] std::string name() const;

    void set_name(const std::string &name);

    [[nodiscard]] int tier() const;

    void set_tier(int tier);

    [[nodiscard]] int attack() const;

    void set_attack(int attack);

    [[nodiscard]] int health() const;

    void set_health(int health);

    [[nodiscard]] int max_health() const;

    void set_max_health(int max_health);

    [[nodiscard]] int aura_attack() const;

    void set_aura_attack(int aura_attack);

    [[nodiscard]] int aura_health() const;

    void set_aura_health(int aura_health);

    void disable_aura(bool permanent = false);

    int deal_damage(int damage);

    int delta_attack(int delta, bool aura = false);

    int delta_health(int delta, bool aura = false);

    [[nodiscard]] const std::unordered_map<Keyword, Effect>& effects() const;

    [[nodiscard]] const Effect& get_effect(Keyword keyword) const;

    [[nodiscard]] int id() const;

    [[nodiscard]] int alt_id() const;

    [[nodiscard]] bool is_golden() const;

    [[nodiscard]] bool is_zombie() const;

    void set_zombie(bool is_zombie);

    [[nodiscard]] bool is_poisoned() const;

    void set_poisoned(bool is_poisoned);

    BitVector<Keyword>& props();

    BitVector<Race>& races();

    [[nodiscard]] bool has(Keyword keyword) const;

    [[nodiscard]] bool is(Race race) const;

    void set(Keyword keyword);

    void clear(Keyword keyword);

    void toggle(Keyword keyword);

    [[nodiscard]] Minion* left_adjacent() const;

    void set_left_adjacent(Minion* left);

    [[nodiscard]] Minion* right_adjacent() const;

    void set_right_adjacent(Minion* right);

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
    int _aura_attack{};
    int _aura_health{};
    int _max_health{};
    BitVector<Keyword> _props;
    BitVector<Race> _races;
    bool _is_zombie{}; // todo: this can be a prop?
    bool _is_poisoned{}; // todo: this can be a prop?

    Minion* _left; // adjacency buff only
    Minion* _right; // adjacency buff only
};

#endif //CARD_H
