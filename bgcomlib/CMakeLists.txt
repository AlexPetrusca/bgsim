cmake_minimum_required(VERSION 3.31)
project(bgcomlib)

set(CMAKE_CXX_STANDARD 26)

add_library(bgcomlib STATIC
        Minion.cpp
        include/card/Minion.h
        Player.cpp
        include/Player.h
        Board.cpp
        include/Board.h
        IPrintable.h
        CardDB.cpp
        include/card/CardDB.h
        Enchantment.cpp
        include/card/Enchantment.h)
target_include_directories(bgcomlib PUBLIC include)

FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)
target_link_libraries(bgcomlib PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(tests)