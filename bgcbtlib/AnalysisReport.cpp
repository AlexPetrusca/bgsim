#include "include/AnalysisReport.h"

#include <algorithm>
#include <sstream>

AnalysisReport::AnalysisReport() {
    this->_winsA = 0;
    this->_winsB = 0;
    this->_total = 0;
    this->_sorted = false;
}

void AnalysisReport::add_battle_report(BattleReport report) {
    this->_total++;
    if (report.result() == WIN_A) {
        this->_winsA++;
        this->_damagesB.push_back(report.damage());
    } else if (report.result() == WIN_B) {
        this->_winsB++;
        this->_damagesA.push_back(report.damage());
    }
}

int AnalysisReport::in_dmg_quart1() {
    const auto& dmg_log = in_dmg_log();
    if (dmg_log.empty()) return 0;
    return dmg_log[dmg_log.size() / 4];
}

int AnalysisReport::in_dmg_median() {
    const auto& dmg_log = in_dmg_log();
    if (dmg_log.empty()) return 0;
    return dmg_log[dmg_log.size() / 2];
}

int AnalysisReport::in_dmg_quart3() {
    const auto& dmg_log = in_dmg_log();
    if (dmg_log.empty()) return 0;
    return dmg_log[3 * dmg_log.size() / 4];
}

int AnalysisReport::out_dmg_quart1() {
    const auto& dmg_log = out_dmg_log();
    if (dmg_log.empty()) return 0;
    return dmg_log[dmg_log.size() / 4];
}

int AnalysisReport::out_dmg_median() {
    const auto& dmg_log = out_dmg_log();
    if (dmg_log.empty()) return 0;
    return dmg_log[dmg_log.size() / 2];
}

int AnalysisReport::out_dmg_quart3() {
    const auto& dmg_log = out_dmg_log();
    if (dmg_log.empty()) return 0;
    return dmg_log[3 * dmg_log.size() / 4];
}

double AnalysisReport::in_dmg_percent_ge(int dmg) {
    const auto& dmg_log = in_dmg_log();
    if (dmg_log.empty()) return 0;

    int ge = 0;
    for (const auto& d : dmg_log) {
        if (d >= dmg) {
            ge++;
        }
    }
    return ge / static_cast<double>(dmg_log.size());
}

double AnalysisReport::out_dmg_percent_ge(int dmg) {
    const auto& dmg_log = out_dmg_log();
    if (dmg_log.empty()) return 0;

    int ge = 0;
    for (const auto& d : dmg_log) {
        if (d >= dmg) {
            ge++;
        }
    }
    return ge / static_cast<double>(dmg_log.size());
}

std::string AnalysisReport::to_string() {
    std::ostringstream oss;
    oss << "\nAnalysis Report" << std::endl;
    oss << "-------------------------------------------------------------" << std::endl;

    const double win_percent = 100 * static_cast<double>(wins()) / _total;
    const double tie_percent = 100 * static_cast<double>(ties()) / _total;
    const double loss_percent = 100 * static_cast<double>(losses()) / _total;
    oss << "Winrate: \t\t";
    oss << wins() << " / " << losses() << " / " << ties();
    oss << "  (" << win_percent << "% / " << tie_percent << "% / " << loss_percent << "%)" << std::endl;

    oss << "Avg Dmg Dealt: \t";
    int out_q1 = out_dmg_quart1();
    int out_q3 = out_dmg_quart3();
    if (out_q1 == out_q3) {
        oss << out_q1 << std::endl;
    } else {
        oss << out_q1 << "-" << out_q3 << std::endl;
    }

    oss << "Avg Dmg Taken: \t";
    int in_q1 = in_dmg_quart1();
    int in_q3 = in_dmg_quart3();
    if (in_q1 == in_q3) {
        oss << in_q1 << std::endl;
    } else {
        oss << in_q1 << "-" << in_q3 << std::endl;
    }

    oss << "-------------------------------------------------------------" << std::endl;
    return oss.str();
}

void AnalysisReport::sort() {
    std::ranges::sort(this->_damagesA);
    std::ranges::sort(this->_damagesB);
    _sorted = true;
}
