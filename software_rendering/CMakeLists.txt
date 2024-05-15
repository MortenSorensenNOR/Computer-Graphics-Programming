cmake_minimum_required(VERSION 3.18)

project(
    software_rendering
    VERSION 1.0
    LANGUAGES C
)

add_subdirectory(display)
add_subdirectory(gml)

add_executable(
    render
    main.c
    RenderUtil.c
)

target_link_libraries(render PRIVATE display gml)
