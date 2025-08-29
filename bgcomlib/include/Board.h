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

    void summon_minion(const Minion& minion);

    void summon_minion(const Minion& minion, MinionLoc loc);

    void kill_minion(MinionLoc loc);

    bool damage_minion(MinionLoc loc, int damage);

    void exec_effect(const Effect& effect, MinionLoc loc);

    void prep_for_battle();

    void increment_active();

    [[nodiscard]] MinionLoc active() const;

    [[nodiscard]] int tier_total() const;

    [[nodiscard]] std::string to_string() override;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool empty() const;

    [[nodiscard]] bool full() const;

    [[nodiscard]] int taunt_count() const;

private:
    std::list<Minion> _minions;
    MinionLoc _active; // todo: should the board really track this
    int _taunt_count;
    int _zombie_count;
    // Aura auras[];
};

#endif //BOARD_H
