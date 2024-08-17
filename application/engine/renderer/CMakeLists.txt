cmake_minimum_required(VERSION 3.18)

add_library(renderer STATIC
    renderer.cpp
)

target_link_libraries(renderer PRIVATE glm::glm)
