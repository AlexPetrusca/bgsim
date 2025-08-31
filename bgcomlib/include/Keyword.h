#ifndef KEYWORD_H
#define KEYWORD_H

enum class Keyword : uint64_t {
    None = 0,

    // Official
    DEATHRATTLE = 1ull << 0,
    REBORN = 1ull << 1,
    BATTLECRY = 1ull << 2,
    POISONOUS = 1ull << 3,
    VENOMOUS = 1ull << 4,
    TAUNT = 1ull << 5,
    DIVINE_SHIELD = 1ull << 6,
    MAGNETIC = 1ull << 7,
    RALLY = 1ull << 8,
    OVERKILL = 1ull << 9,
    WINDFURY = 1ull << 10,
    MEGA_WINDFURY = 1ull << 11,
    STEALTH = 1ull << 12,
    CHOOSE_ONE = 1ull << 13,

    // Custom
    CLEAVE = 1ull << 14,
    IMMUNE = 1ull << 15,
    AURA = 1ull << 16,
    ADJACENT_BUFF = 1ull << 17,
    START_OF_TURN = 1ull << 18,
    END_OF_TURN = 1ull << 19,
    ON_DAMAGE_SELF = 1ull << 20,
    ON_DAMAGE_OTHER = 1ull << 21,
    ON_DAMAGE_PLAYER = 1ull << 22,
    ON_SUMMON = 1ull << 23,
    ON_ATTACK = 1ull << 24,
    ON_KILL = 1ull << 25,
    ON_DEATH_OTHER = 1ull << 26,

    // Card-Specific
    ZAPP_SLYWICK = 1ull << 27,
};

namespace KeywordUtil {
    inline std::map<std::string, Keyword> keyword_map = {
        {"DEATHRATTLE", Keyword::DEATHRATTLE},
        {"REBORN", Keyword::REBORN},
        {"BATTLECRY", Keyword::BATTLECRY},
        {"POISONOUS", Keyword::POISONOUS},
        {"VENOMOUS", Keyword::VENOMOUS},
        {"TAUNT", Keyword::TAUNT},
        {"DIVINE_SHIELD", Keyword::DIVINE_SHIELD},
        {"MAGNETIC", Keyword::MAGNETIC},
        {"RALLY", Keyword::RALLY},
        {"OVERKILL", Keyword::OVERKILL},
        {"WINDFURY", Keyword::WINDFURY},
        {"MEGA_WINDFURY", Keyword::MEGA_WINDFURY},
        {"STEALTH", Keyword::STEALTH},
        {"CHOOSE_ONE", Keyword::CHOOSE_ONE},

        {"CLEAVE", Keyword::CLEAVE},
        {"IMMUNE", Keyword::IMMUNE},
        {"AURA", Keyword::AURA},
        {"ADJACENT_BUFF", Keyword::ADJACENT_BUFF},
        {"START_OF_TURN", Keyword::START_OF_TURN},
        {"END_OF_TURN", Keyword::END_OF_TURN},
        {"ON_DAMAGE_SELF", Keyword::ON_DAMAGE_SELF},
        {"ON_DAMAGE_OTHER", Keyword::ON_DAMAGE_OTHER},
        {"ON_DAMAGE_PLAYER", Keyword::ON_DAMAGE_PLAYER},
        {"ON_SUMMON", Keyword::ON_SUMMON},
        {"ON_ATTACK", Keyword::ON_ATTACK},
        {"ON_KILL", Keyword::ON_KILL},
        {"ON_DEATH_OTHER", Keyword::ON_DEATH_OTHER},

        {"ZAPP_SLYWICK", Keyword::ZAPP_SLYWICK},
    };

    inline Keyword fromString(const std::string& str) {
        return keyword_map.find(str)->second;
    }
}

template <typename E>
class BitVector {
    using U = std::underlying_type_t<E>;
    U bits;
public:
    BitVector() : bits(0) {}
    explicit BitVector(E e) : bits(static_cast<U>(e)) {}
    explicit BitVector(U val) : bits(val) {}

    bool operator[](E e) const {
        return bits & static_cast<U>(e);
    }

    bool has(E e) const {
        return bits & static_cast<U>(e);
    }

    void set(E e) {
        bits |= static_cast<U>(e);
    }

    void clear(E e) {
        bits &= ~static_cast<U>(e);
    }

    [[nodiscard]] bool any() const {
        return bits != 0;
    }
};

#endif //KEYWORD_H
