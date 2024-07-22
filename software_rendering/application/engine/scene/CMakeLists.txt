cmake_minimum_required(VERSION 3.18)

find_package(glm CONFIG REQUIRED)

add_library(scene STATIC
    scene.cpp
    camera.cpp
    tinyxml2/tinyxml2.cpp
)

target_link_libraries(scene PRIVATE glm)
