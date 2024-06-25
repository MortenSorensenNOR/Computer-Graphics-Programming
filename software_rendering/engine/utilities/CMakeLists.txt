# install libassimp-dev
find_package(assimp REQUIRED)

include_directories(
    ${Assimp_SOURCE_DIR}/include
    ${Assimp_SOURCE_DIR}/code
)

link_directories (
    ${Assimp_BINARY_DIR}
    ${Assimp_BINARY_DIR}/lib
)

add_library(utilities STATIC
    RenderUtil.c 
    ModelLoader.cpp
)

target_include_directories(utilities PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(utilities PUBLIC assimp m)
