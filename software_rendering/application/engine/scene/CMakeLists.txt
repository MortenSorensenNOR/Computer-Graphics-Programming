cmake_minimum_required(VERSION 3.18)

include_directories(
    ${Assimp_SOURCE_DIR}/include
    ${Assimp_SOURCE_DIR}/code
)

link_directories (
    ${Assimp_BINARY_DIR}
    ${Assimp_BINARY_DIR}/lib
)

add_library(scene STATIC
    scene.cpp
    camera.cpp
    asset_load.cpp
    tinyxml2/tinyxml2.cpp
)

target_link_libraries(scene PRIVATE glm::glm assimp)
