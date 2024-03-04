cmake_minimum_required(VERSION 3.18)

project(
    GPU_Emulator
    VERSION 1.0
    LANGUAGES CXX
)

add_executable(
    Emulate
    main.cpp
)

target_include_directories(Emulate PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Emulate PRIVATE X11 stdc++)
