cmake_minimum_required(VERSION 3.18)

add_subdirectory(core_utils)
add_subdirectory(display)
add_subdirectory(engine)

add_library(application STATIC
    application.cpp
)

target_link_libraries(application PRIVATE core_utils display engine glm::glm SDL2::SDL2 OpenMP::OpenMP_CXX assimp)
