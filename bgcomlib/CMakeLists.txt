cmake_minimum_required(VERSION 3.31)
project(bgcomlib)

set(CMAKE_CXX_STANDARD 26)

add_library(bgcomlib STATIC
        card/Minion.cpp
        Player.cpp
        Board.cpp
        IPrintable.h
        card/CardDb.cpp
        card/Enchantment.cpp
        include/card/Minion.h
        include/Player.h
        include/Board.h
        include/card/CardDB.h
        include/card/Enchantment.h
        include/Keyword.h
        include/Effect.h
        Effect.cpp
        include/Target.h
        include/Race.h
        include/MinionLoc.h
        include/util/BitVector.h
        include/Hand.h
        Hand.cpp
        include/util/Random.h
        util/Random.cpp
        Pool.cpp
        include/Pool.h
)
target_include_directories(bgcomlib PUBLIC include)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)
target_link_libraries(bgcomlib PUBLIC nlohmann_json::nlohmann_json)

add_subdirectory(tests)