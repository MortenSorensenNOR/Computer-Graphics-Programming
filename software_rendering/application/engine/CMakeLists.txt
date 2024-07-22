cmake_minimum_required(VERSION 3.18)

find_package(glm CONFIG REQUIRED)

add_subdirectory(scene)

add_library(engine STATIC
    engine.cpp
)

target_link_libraries(engine PRIVATE scene)

