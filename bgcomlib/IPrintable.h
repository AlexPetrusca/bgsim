#ifndef IPRINTABLE_H
#define IPRINTABLE_H

#include <iosfwd>
#include <string>

class IPrintable {
public:
    virtual ~IPrintable() = default;

    [[nodiscard]] virtual std::string to_string() = 0;

    friend std::ostream& operator<<(std::ostream& os, IPrintable& p) {
        os << p.to_string();
        return os;
    }
};

#endif //IPRINTABLE_H
