#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <unordered_map>

enum class Target {
    SINGLE,
    SELF,
    ALL,
    ALL_OTHER,
    LEFTMOST,
    RIGHTMOST,
    ONE_OF_EACH,
};

namespace TargetUtil {
    inline std::unordered_map<std::string, Target> target_map = {
        {"SINGLE", Target::SINGLE},
        {"SELF", Target::SELF},
        {"ALL", Target::ALL},
        {"ALL_OTHER", Target::ALL_OTHER},
        {"LEFTMOST", Target::LEFTMOST},
        {"RIGHTMOST", Target::RIGHTMOST},
        {"ONE_OF_EACH", Target::ONE_OF_EACH},
    };

    inline Target fromString(const std::string& str) {
        return target_map.find(str)->second;
    }
}

#endif //TARGET_H
