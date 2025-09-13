#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <random>
#include <unordered_set>

#include "card/Minion.h"
#include "card/CardDb.h"
#include "../IPrintable.h"

// todo: move into separate file
using MinionLoc = std::list<Minion>::iterator;

struct MinionLocHash {
    std::size_t operator()(const std::list<Minion>::iterator& it) const noexcept {
        return std::hash<Minion*>{}(&*it); // hash the pointer to the element
    }
};

struct MinionLocEq {
    bool operator()(const std::list<Minion>::iterator& a, const std::list<Minion>::iterator& b) const noexcept {
        return &*a == &*b;
    }
};

using MinionLocSet = std::unordered_set<MinionLoc, MinionLocHash, MinionLocEq>;

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

    void add_minion(const Minion& minion);

    void add_minion(const Minion& minion, MinionLoc loc);

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

    void proc_trigger(Keyword trigger, MinionLoc source);

    void register_trigger(Keyword trigger, MinionLoc loc);

    void deregister_trigger(Keyword trigger, MinionLoc loc);

    void deregister_triggers(MinionLoc loc);

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
    MinionLoc _active;
    std::unordered_map<Keyword, MinionLocSet> _triggers;
    int _taunt_count{};
    int _zombie_count{};
    std::mt19937 _rng;
};

#endif //BOARD_H
