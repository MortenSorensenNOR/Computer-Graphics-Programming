cmake_minimum_required(VERSION 3.18)

add_library(tiled_renderer STATIC
    tiled_renderer.cpp
    vertex_shader.cpp
    vertex_post_processor.cpp
    primitive_assembler.cpp
    primitive_binner.cpp
    rasterizer.cpp
    fragment_shader.cpp
    attribute_interpolation.cpp
)

target_link_libraries(tiled_renderer PRIVATE glm::glm)
