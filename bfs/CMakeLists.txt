cmake_minimum_required(VERSION 3.16)
project(CW2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)

FetchContent_Declare(
        parlaylib
        GIT_REPOSITORY https://github.com/cmuparlay/parlaylib.git
        GIT_TAG master
)

set(PARLAY_HEADER_ONLY OFF CACHE BOOL "" FORCE)

FetchContent_GetProperties(parlaylib)
if(NOT parlaylib_POPULATED)
    FetchContent_Populate(parlaylib)
    add_subdirectory(${parlaylib_SOURCE_DIR} EXCLUDE_FROM_ALL)
endif()

add_executable(CW2
        main.cpp
        test.h
        seq.h
        par.h
        graph_helper.h
)

target_link_libraries(CW2 PRIVATE parlay)
