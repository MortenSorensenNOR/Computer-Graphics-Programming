cmake_minimum_required(VERSION 3.18)

add_library(core_utils
    core_utils.cpp
)

target_include_directories(core_utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(core_utils PRIVATE glm::glm)
