#ifndef BOARD_H
#define BOARD_H

#include "card/Minion.h"
#include "card/CardDb.h"
#include "../IPrintable.h"

class Board : public IPrintable {
public:
    explicit Board(const std::vector<Minion>& minions = {});

    static Board from_ids(const std::vector<CardDb::Id>& minionIds);

    std::vector<Minion>& get_minions();

    void summon_minion(const Minion& minion, size_t idx = -1);

    void kill_minion(size_t idx);

    bool damage_minion(size_t idx, int damage);

    void exec_effect(const Effect& effect, size_t idx);

    [[nodiscard]] int tier_total() const;

    [[nodiscard]] std::string to_string() override;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool empty() const;

    [[nodiscard]] bool full() const;

    [[nodiscard]] int taunt_count() const;

private:
    std::vector<Minion> _minions;
    int _taunt_count;
    // Aura auras[];
};

#endif //BOARD_H
