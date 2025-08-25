cmake_minimum_required(VERSION 3.31)
project(bgsimroot)

set(CMAKE_CXX_STANDARD 26)

enable_testing()

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.17.0
)
FetchContent_MakeAvailable(googletest)

add_compile_definitions(
        ROOT_DIR="${CMAKE_SOURCE_DIR}"
        BGCOMLIB_DIR="${CMAKE_SOURCE_DIR}/bgcomlib"
        BGCBTLIB_DIR="${CMAKE_SOURCE_DIR}/bgcbtlib"
)

add_subdirectory(bgcomlib)
add_subdirectory(bgcbtlib)
add_subdirectory(bgsim-server)
