cmake_minimum_required(VERSION 3.31)
project(bgsim_server)

set(CMAKE_CXX_STANDARD 26)

add_executable(bgsim_server main.cpp
        Game.cpp
        Game.h)
target_link_libraries(bgsim_server PRIVATE bgcomlib)
target_include_directories(bgcomlib PUBLIC include)
