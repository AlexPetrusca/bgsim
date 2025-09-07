#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <random>

#include "card/Minion.h"
#include "card/CardDb.h"
#include "../IPrintable.h"

using MinionLoc = std::list<Minion>::iterator;

class Board : public IPrintable {
public:
    explicit Board(const std::vector<Minion>& minions = {});

    static Board from_ids(const std::vector<CardDb::Id>& minionIds);

    std::list<Minion>& minions();

    MinionLoc get_random_minion_loc();

    MinionLoc get_random_minion_loc(const BitVector<Keyword>& exclude);

    MinionLoc get_random_minion_loc_by_race(Race race);

    MinionLoc get_left_minion_loc(MinionLoc loc);

    static MinionLoc get_right_minion_loc(MinionLoc loc);

    bool is_minion(MinionLoc loc);

    void summon_minion(const Minion& minion, bool post_death = false);

    void summon_minion(const Minion& minion, MinionLoc loc, bool post_death = false);

    void enchant_minion(Minion& minion, const Enchantment& enchantment);

    void enchant_random_minion(const Enchantment& enchantment);

    void enchant_random_minion_by_race(const Enchantment& enchantment, Race race);

    void reap_minion(MinionLoc loc);

    int damage_minion(MinionLoc loc, int damage, bool poisoned = false);

    bool try_reap_minion(MinionLoc loc);

    void exec_effect(const Effect& effect, MinionLoc loc);

    void prep_for_battle();

    void increment_active();

    [[nodiscard]] MinionLoc active() const;

    [[nodiscard]] int tier_total() const;

    [[nodiscard]] size_t size(bool include_zombies = false) const;

    [[nodiscard]] bool empty(bool include_zombies = false) const;

    [[nodiscard]] bool full(bool include_zombies = false) const;

    void set_rng(const std::mt19937& rng);

    [[nodiscard]] int taunt_count() const;

    [[nodiscard]] int zombie_count() const;

    [[nodiscard]] std::string to_string() override;

private:
    std::list<Minion> _minions;
    MinionLoc _active; // todo: should the board really track this
    int _taunt_count;
    int _zombie_count;
    std::mt19937 _rng;
};

#endif //BOARD_H
