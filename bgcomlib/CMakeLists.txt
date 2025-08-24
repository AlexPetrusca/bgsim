cmake_minimum_required(VERSION 3.31)
project(bgcomlib)

set(CMAKE_CXX_STANDARD 26)

add_library(bgcomlib STATIC
        Minion.cpp
        include/Minion.h
        Player.cpp
        include/Player.h
        Board.cpp
        include/Board.h
        IPrintable.h)
target_include_directories(bgcomlib PUBLIC include)

add_subdirectory(tests)