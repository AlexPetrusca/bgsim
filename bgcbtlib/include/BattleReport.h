#ifndef BATTLEREPORT_H
#define BATTLEREPORT_H

#include "BattleStatus.h"

class BattleReport {
public:
    BattleReport(BattleStatus result, int damage);

    [[nodiscard]] BattleStatus result() const {
        return _result;
    }

    [[nodiscard]] int damage() const {
        return _damage;
    }

private:
    BattleStatus _result;
    int _damage;
};

#endif //BATTLEREPORT_H
