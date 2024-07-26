cmake_minimum_required(VERSION 3.18)

add_subdirectory(scene)
add_subdirectory(renderer)

add_library(engine STATIC
    engine.cpp
)

target_link_libraries(engine PRIVATE scene renderer glm::glm)

