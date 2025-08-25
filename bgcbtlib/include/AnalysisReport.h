#ifndef ANALYSISREPORT_H
#define ANALYSISREPORT_H

#include <vector>

#include "BattleReport.h"
#include "../../bgcomlib/IPrintable.h"

class AnalysisReport : public IPrintable {
public:
    AnalysisReport();
    void add_battle_report(BattleReport report);

    [[nodiscard]] int wins() const {
        return _winsA;
    }

    [[nodiscard]] int losses() const {
        return _winsB;
    }

    [[nodiscard]] int ties() const {
        return _total - _winsA - _winsB;
    }

    [[nodiscard]] int total() const {
        return _total;
    }

    [[nodiscard]] std::vector<int>& in_dmg_log() {
        if (!_sorted) sort();
        return _damagesA;
    }

    [[nodiscard]] std::vector<int>& out_dmg_log() {
        if (!_sorted) sort();
        return _damagesB;
    }

    [[nodiscard]] int in_dmg_quart1();

    [[nodiscard]] int in_dmg_median();

    [[nodiscard]] int in_dmg_quart3();

    [[nodiscard]] int out_dmg_quart1();

    [[nodiscard]] int out_dmg_median();

    [[nodiscard]] int out_dmg_quart3();

    [[nodiscard]] double in_dmg_percent_ge(int dmg);

    [[nodiscard]] double out_dmg_percent_ge(int dmg);

    [[nodiscard]] std::string to_string() override;

private:
    int _winsA;
    int _winsB;
    int _total;
    std::vector<int> _damagesA;
    std::vector<int> _damagesB;
    bool _sorted;

    void sort();
};

#endif //ANALYSISREPORT_H
