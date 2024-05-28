cmake_minimum_required(VERSION 3.18)

project(
    software_rendering
    VERSION 1.0
    LANGUAGES C
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -O0")

add_subdirectory(display)
add_subdirectory(gml)
add_subdirectory(rutil)

add_executable(
    render
    main.c
)

target_link_libraries(render PRIVATE display gml rutil)
