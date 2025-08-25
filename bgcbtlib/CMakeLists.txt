cmake_minimum_required(VERSION 3.31)
project(bgcbtlib)

set(CMAKE_CXX_STANDARD 26)

add_library(bgcbtlib STATIC
        Arena.cpp
        include/Arena.h
        BattleReport.cpp
        include/BattleReport.h
        AnalysisReport.cpp
        include/AnalysisReport.h
        include/BattleReport.h)
target_link_libraries(bgcbtlib PRIVATE bgcomlib)
target_include_directories(bgcomlib PUBLIC include)

add_subdirectory(tests)