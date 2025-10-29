#include "include/Player.h"

Player::Player() {
    _health = 30;
    _max_health = _health;
    _armor = 10;
    _board.bind_player(this);
}

Board& Player::board() {
    return _board;
}

void Player::set_board(Board& board) {
    _board = std::move(board);
    _board.bind_player(this);
}

Hand& Player::hand() {
    return _hand;
}

Discovers& Player::discovers() {
    return _discovers;
}

int Player::health() const {
    return _health;
}

int Player::armor() const {
    return _armor;
}

int Player::missing_health() const {
    return _max_health - _health;
}

int Player::total_health() {
    return _health + _armor;
}

void Player::deal_damage(const int damage) {
    if (_is_immune) return;

    if (damage < _armor) {
        _armor -= damage;
    } else {
        _health -= damage - _armor;
        _armor = 0;
    }
}

void Player::set_health(const int health) {
    _health = health;
}

void Player::set_armor(const int armor) {
    _armor = armor;
}

void Player::increment_pogo_counter() {
    _pogo_counter++;
}

int Player::pogo_counter() {
    return _pogo_counter;
}

bool Player::is_immune() {
    return _is_immune;
}

void Player::set_immune(const bool is_immune) {
    _is_immune = is_immune;
}

void Player::set_tier(const int tier) {
    _tier = tier;
}

int Player::tier() {
    return _tier;
}

void Player::select_discover(const int idx) {
    if (_hand.full()) return;

    std::shared_ptr<Card> discover = _discovers.select(idx);
    if (CardUtil::is_minion(discover)) {
        _hand.add_minion(CardUtil::as_minion(discover));
        _board.proc_trigger(Keyword::DISCOVER);
    } else if (CardUtil::is_enchantment(discover)) {
        _board.proc_trigger(Keyword::ADAPT);
    }
}

Player* Player::opponent() {
    return _opponenet;
}

Pool* Player::pool() {
    return _pool;
}

void Player::bind_opponent(Player* opponenet) {
    _opponenet = opponenet;
}

void Player::bind_pool(Pool* pool) {
    _pool = pool;
    _discovers.bind_pool(pool);
}

void Player::unbind_opponent() {
    _opponenet = nullptr;
}
