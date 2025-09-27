#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <iterator>
#include <type_traits>

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

    BitVector& operator|=(BitVector v) {
        bits |= v.bits;
        return *this;
    }

    BitVector& operator|=(U v) {
        bits |= v;
        return *this;
    }

    BitVector& operator&=(BitVector v) {
        bits &= v.bits;
        return *this;
    }

    BitVector& operator&=(U v) {
        bits &= v;
        return *this;
    }

    U operator|(BitVector v) {
        return bits | v.bits;
    }

    BitVector<Keyword>& operator|(U v) {
        bits |= v;
        return *this;
    }

    U operator&(BitVector v) {
        return bits & v.bits;
    }

    BitVector<Keyword>& operator&(U v) {
        bits &= v;
        return *this;
    }

    U operator~() const {
        return ~bits;
    }

    BitVector& operator=(const BitVector& other) {
        if (this != &other) {
            bits = other.bits;
        }
        return *this;
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

    [[nodiscard]] bool empty() const {
        return bits == 0;
    }

    [[nodiscard]] bool intersects(BitVector v) const {
        return bits & v.bits;
    }

    [[nodiscard]] int size() const {
        int count = 0;
        int n = bits;
        while (n > 0) {
            n &= n - 1;
            count += 1;
        }
        return count;
    }

    class iterator {
        U remaining;
    public:
        using value_type = E;
        using reference = E;
        using pointer = void;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::input_iterator_tag;

        explicit iterator(U bits) : remaining(bits) {}

        E operator*() const {
            U lowest = remaining & -remaining;
            return static_cast<E>(lowest);
        }

        iterator& operator++() {
            remaining &= (remaining - 1);
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return remaining != other.remaining;
        }
    };

    iterator begin() const {
        return iterator(bits);
    }

    iterator end() const {
        return iterator(0);
    }
};

#endif //BITVECTOR_H
