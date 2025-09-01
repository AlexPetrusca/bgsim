#ifndef BOARD_H
#define BOARD_H

#include <list>

#include "card/Minion.h"
#include "card/CardDb.h"
#include "../IPrintable.h"

using MinionLoc = std::list<Minion>::iterator;

class Board : public IPrintable {
public:
    explicit Board(const std::vector<Minion>& minions = {});

    static Board from_ids(const std::vector<CardDb::Id>& minionIds);

    std::list<Minion>& minions();

    void summon_minion(const Minion& minion, bool post_death = false);

    void summon_minion(const Minion& minion, MinionLoc loc, bool post_death = false);

    void reap_minion(MinionLoc loc);

    void damage_minion(MinionLoc loc, int damage);

    bool try_reap_minion(MinionLoc loc);

    void exec_effect(const Effect& effect, MinionLoc loc);

    void prep_for_battle();

    void increment_active();

    [[nodiscard]] MinionLoc active() const;

    [[nodiscard]] int tier_total() const;

    [[nodiscard]] size_t size(bool include_zombies = false) const;

    [[nodiscard]] bool empty(bool include_zombies = false) const;

    [[nodiscard]] bool full(bool include_zombies = false) const;

    [[nodiscard]] int taunt_count() const;

    [[nodiscard]] int zombie_count() const;

    [[nodiscard]] std::string to_string() override;

private:
    std::list<Minion> _minions;
    MinionLoc _active; // todo: should the board really track this
    int _taunt_count;
    int _zombie_count;
    // Aura auras[];
};

#endif //BOARD_H
