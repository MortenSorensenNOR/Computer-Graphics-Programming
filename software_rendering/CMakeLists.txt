cmake_minimum_required(VERSION 3.18)

project(
    software_rendering
    VERSION 1.0
    LANGUAGES C
)

add_subdirectory(display)
add_subdirectory(gml)
add_subdirectory(rutil)

add_executable(
    render
    main.c
)

target_link_libraries(render PRIVATE display gml rutil)
