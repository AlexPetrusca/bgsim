#include "include/Arena.h"

#include <iostream>
#include <sstream>

#include "util/Random.h"

Arena::Arena() = default;

Arena::Arena(const Player& p1, const Player& p2) {
    this->_p1 = p1;
    this->_p2 = p2;
}

Arena Arena::from_boards(Board& b1, Board& b2) {
    Arena arena;
    arena.playerA().set_board(b1);
    arena.playerB().set_board(b2);
    return arena;
}

BattleStatus Arena::get_battle_status() {
    if (_p1.board().empty() && _p2.board().empty()) return TIE;
    if (_p1.board().empty()) return WIN_B;
    if (_p2.board().empty()) return WIN_A;

    for (const auto& minion: _p1.board().minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }
    for (const auto& minion: _p2.board().minions()) {
        if (minion.attack() != 0) return IN_PROGRESS;
    }

    return TIE;
}

// todo: refactor this code out to Board
void fight_minions(Board& attacking, Board& defending, const MinionLoc atk, const MinionLoc def) {
    attacking.proc_trigger(Keyword::ON_ATTACK, &*atk);

    const int def_attack = def->attack();
    const int atk_attack = atk->attack();

    const bool def_poisoned = def->has(Keyword::POISONOUS) || def->has(Keyword::VENOMOUS);
    const int def_damage_dealt = attacking.damage_minion(atk, def_attack, def_poisoned);
    if (def_damage_dealt > 0 && def->has(Keyword::VENOMOUS)) {
        def->clear(Keyword::VENOMOUS);
    }

    const bool atk_poisoned = atk->has(Keyword::POISONOUS) || atk->has(Keyword::VENOMOUS);
    int atk_damage_dealt = 0;
    // todo: rewrite using `get_left_minion_loc` and `get_right_minion_loc`
    if (atk->has(Keyword::CLEAVE)) {
        // attack left
        if (def != defending.minions().begin()) { // if not attacking head
            const auto ours_left = std::prev(def);
            atk_damage_dealt += defending.damage_minion(ours_left, atk_attack, atk_poisoned);
        }
        // attack middle
        atk_damage_dealt += defending.damage_minion(def, atk_attack, atk_poisoned);
        // attack right
        const auto ours_right = std::next(def);
        if (ours_right != defending.minions().end()) { // if not attacking tail
            atk_damage_dealt += defending.damage_minion(ours_right, atk_attack, atk_poisoned);
        }
    } else {
        atk_damage_dealt = defending.damage_minion(def, atk_attack, atk_poisoned);
    }
    if (atk_damage_dealt > 0 && atk->has(Keyword::VENOMOUS)) {
        atk->clear(Keyword::VENOMOUS);
    }
}

// todo: refactor most of this code out to Board
void Arena::combat(const int turn, const bool debug) {
    Board& attacking = turn % 2 == 0 ? _p1.board() : _p2.board();
    Board& defending = turn % 2 == 0 ? _p2.board() : _p1.board();

    // select attacker
    const MinionLoc atk_minion = attacking.active();

    int attack_count = 1;
    if (atk_minion->has(Keyword::WINDFURY)) {
        attack_count = 2;
    } else if (atk_minion->has(Keyword::MEGA_WINDFURY)) {
        attack_count = 4;
    }

    bool attacker_died = false;
    for (int i = 0; i < attack_count; i++) {
        // select defender
        MinionLoc def_minion;
        if (defending.taunt_count() > 0) {
            int taunt_idx = rng.rand_int(defending.taunt_count() - 1);
            for (auto m = defending.minions().begin(); m != defending.minions().end(); ++m) {
                if (m->has(Keyword::TAUNT)) {
                    if (taunt_idx == 0) {
                        def_minion = m;
                        break;
                    }
                    taunt_idx--;
                }
            }
        } else {
            const int minion_idx = rng.rand_int(defending.size() - 1);
            def_minion = std::next(defending.minions().begin(), minion_idx);
        }

        if (debug) {
            std::cout << "|Turn " << turn << "| ";
            if (turn % 2 == 0) {
                std::cout << "[A -> B] ";
            } else {
                std::cout << "[B -> A] ";
            }
            int atk_minion_idx = -1;
            for (auto m = attacking.minions().begin(); m != attacking.minions().end(); ++m) {
                atk_minion_idx++;
                if (m == atk_minion) {
                    break;
                }
            }
            int def_minion_idx = -1;
            for (auto m = defending.minions().begin(); m != defending.minions().end(); ++m) {
                def_minion_idx++;
                if (m == def_minion) {
                    break;
                }
            }
            std::cout << "\"" << atk_minion->name() << "\"" << " (" << atk_minion_idx << ")";
            std::cout << " -> ";
            std::cout << "\"" << def_minion->name() << "\"" << " (" << def_minion_idx << ")";
            std::cout << std::endl;
            std::cout << this->to_string();
        }

        // resolve damage
        fight_minions(attacking, defending, atk_minion, def_minion);

        // resolve deaths
        const bool is_cleave = atk_minion->has(Keyword::CLEAVE);
        if (is_cleave) {
            // todo: trying to reap all is inefficient
            auto m = defending.minions().begin();
            while (m != defending.minions().end()) {
                const auto m_next = std::next(m);
                if (m->is_zombie()) {
                    attacking.proc_trigger(Keyword::ON_KILL, &*atk_minion);
                    if (m->health() < 0) {
                        attacking.proc_trigger(Keyword::OVERKILL, &*atk_minion);
                    }
                }
                defending.try_reap_minion(m);
                m = m_next;
            }
        } else {
            if (def_minion->is_zombie()) {
                attacking.proc_trigger(Keyword::ON_KILL, &*atk_minion);
                if (def_minion->health() < 0) {
                    attacking.proc_trigger(Keyword::OVERKILL, &*atk_minion);
                }
            }
            defending.try_reap_minion(def_minion);
        }

        attacker_died = attacking.try_reap_minion(atk_minion);
        if (attacker_died) {
            break;
        }
    }

    if (!attacker_died) {
        attacking.increment_active();
    }

    if (debug) {
        std::cout << this->to_string();
        std::cout << std::endl;
    }
}

BattleReport Arena::battle(const bool debug, const bool analysis) {
    // bind opponent
    _p1.bind_opponent(&_p2);
    _p2.bind_opponent(&_p1);

    // save
    const Player player_save_a = playerA();
    const Player player_save_b = playerB();

    int turn = _p1.board().size() > _p2.board().size() ? 0 : 1;
    if (_p1.board().size() == _p2.board().size()) {
        turn = rng.coin_flip();
    }

    if (debug) {
        std::cout << std::endl;
    }

    _p1.board().start_of_combat();
    _p2.board().start_of_combat();
    while (get_battle_status() == IN_PROGRESS) {
        _p1.board().pre_attack();
        _p2.board().pre_attack();
        combat(turn, debug);
        _p1.board().post_attack();
        _p2.board().post_attack();
        turn++;
    }

    if (debug) {
        std::cout << "Final Board:" << std::endl;;
        std::cout << this->to_string();
    }

    const BattleStatus status = get_battle_status();
    int damage = 0;
    if (status == WIN_A) {
        damage = _p1.board().tier_total();
    } else if (status == WIN_B) {
        damage = _p2.board().tier_total();
    }
    const BattleReport report = BattleReport(status, damage);

    // restore
    if (analysis) {
        _p1 = player_save_a;
        _p2 = player_save_b;
    }

    // unbind opponent
    _p1.unbind_opponent();
    _p2.unbind_opponent();

    return report;
}

AnalysisReport Arena::analyze(int iterations) {
    AnalysisReport analysis_report = AnalysisReport();
    for (int i = 0; i < iterations; i++) {
        const BattleReport battle_report = battle(false, true);
        analysis_report.add_battle_report(battle_report);
    }
    return analysis_report;
}

void Arena::bind_pool(Pool* pool) {
    _p1.bind_pool(pool);
    _p2.bind_pool(pool);
}

Player& Arena::playerA() {
    return _p1;
}

Player& Arena::playerB() {
    return _p2;
}

std::string Arena::to_string() {
    std::ostringstream oss;
    oss << "-------------------------------------------------------------" << std::endl;
    oss << "[A] " << _p1.board() << std::endl;
    oss << "[B] " << _p2.board() << std::endl;
    oss << "-------------------------------------------------------------" << std::endl;
    return oss.str();
}
