#ifndef TARGET_H
#define TARGET_H

#include <map>
#include <string>

enum class Target {
    SINGLE,
    ALL,
    LEFTMOST,
    RIGHTMOST,
};

namespace TargetUtil {
    inline std::map<std::string, Target> target_map = {
        {"SINGLE", Target::SINGLE},
        {"ALL", Target::ALL},
        {"LEFTMOST", Target::LEFTMOST},
        {"RIGHTMOST", Target::RIGHTMOST},
    };

    inline Target fromString(const std::string& str) {
        return target_map.find(str)->second;
    }
}

#endif //TARGET_H
