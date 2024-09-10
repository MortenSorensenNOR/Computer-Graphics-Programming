cmake_minimum_required(VERSION 3.18)

project(
    software_rendering
    VERSION 1.0
    LANGUAGES C
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
endif()

add_subdirectory(display)
add_subdirectory(gml)
add_subdirectory(rutil)
add_subdirectory(render)

add_executable(
    app
    main.c
)

target_link_libraries(app PRIVATE display gml rutil render)
