add_library(render STATIC
    VertexShader.c
    VertexPostProcessor.c
    Rasterizer.c
    FragmentShader.c
)

target_include_directories(render PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(render PUBLIC rutil gml)
