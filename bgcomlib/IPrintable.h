#ifndef IPRINTABLE_H
#define IPRINTABLE_H

#include <iosfwd>
#include <string>

class IPrintable {
public:
    virtual ~IPrintable() = default;

    [[nodiscard]] virtual std::string to_string() const = 0;   // Pure virtual function

    friend std::ostream& operator<<(std::ostream& os, const IPrintable& p) {
        os << p.to_string();
        return os;
    }
};

#endif //IPRINTABLE_H
