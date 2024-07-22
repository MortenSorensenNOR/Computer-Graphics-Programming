cmake_minimum_required(VERSION 3.18)

find_package(glm CONFIG REQUIRED)

add_subdirectory(
    render_backend
)

add_library(renderer STATIC
    renderer.cpp
)

target_link_libraries(renderer PRIVATE gml render_backend)

