#ifndef BOARD_H
#define BOARD_H

#include <list>
#include <random>
#include <unordered_set>

#include "MinionLoc.h"
#include "card/Minion.h"
#include "card/CardDb.h"
#include "../IPrintable.h"

class Player;

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

    MinionLoc add_minion(const Minion& minion, MinionLoc loc);

    MinionLoc play_minion(const Minion& minion);

    MinionLoc play_minion(const Minion& minion, MinionLoc loc);

    MinionLoc summon_minion(const Minion& minion, bool post_death = false);

    MinionLoc summon_minion(const Minion& minion, MinionLoc loc, bool post_death = false);

    void proc_enchantment(int enchantment_id, MinionLoc source, Minion* target = nullptr);

    void enchant_minion(Minion& minion, const Enchantment& enchantment, bool aura = false);

    void disenchant_minion(Minion& minion, const Enchantment& enchantment, bool aura = false);

    MinionLoc enchant_random_minion(const Enchantment& enchantment);

    MinionLoc enchant_random_minion_by_race(const Enchantment& enchantment, Race race);

    void reap_minion(MinionLoc loc);

    int damage_minion(MinionLoc loc, int damage, bool poisoned = false, bool reap = false);

    bool try_reap_minion(MinionLoc loc);

    void exec_effect(const Effect& effect, MinionLoc source, Minion* target = nullptr);

    void apply_adjacent_aura(MinionLoc loc);

    void undo_adjacent_aura(MinionLoc loc);

    void apply_aura(MinionLoc loc);

    void undo_aura(MinionLoc loc);

    void turn_start();

    void turn_end();

    void pre_combat();

    void post_combat();

    void pre_battle();

    void increment_active();

    void proc_trigger(Keyword trigger, Minion* source = nullptr);

    void register_trigger(Keyword trigger, MinionLoc loc);

    void deregister_trigger(Keyword trigger, MinionLoc loc);

    void deregister_triggers(MinionLoc loc);

    [[nodiscard]] MinionLoc active() const;

    [[nodiscard]] int tier_total() const;

    [[nodiscard]] size_t size(bool include_zombies = false) const;

    [[nodiscard]] bool empty(bool include_zombies = false) const;

    [[nodiscard]] bool full(bool include_zombies = false) const;

    [[nodiscard]] int taunt_count() const;

    [[nodiscard]] int zombie_count() const;

    void bind_player(Player* player);

    [[nodiscard]] std::string to_string() override;

private:
    std::list<Minion> _minions;
    MinionLoc _active;
    std::unordered_map<Keyword, MinionLocSet> _triggers;
    int _taunt_count{};
    int _zombie_count{};

    Player* _player;
};

#endif //BOARD_H
