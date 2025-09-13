#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <unordered_map>

enum class Target {
    SINGLE,
    ALL,
    SELF,
    LEFTMOST,
    RIGHTMOST,
};

namespace TargetUtil {
    inline std::unordered_map<std::string, Target> target_map = {
        {"SINGLE", Target::SINGLE},
        {"ALL", Target::ALL},
        {"SELF", Target::SELF},
        {"LEFTMOST", Target::LEFTMOST},
        {"RIGHTMOST", Target::RIGHTMOST},
    };

    inline Target fromString(const std::string& str) {
        return target_map.find(str)->second;
    }
}

#endif //TARGET_H
