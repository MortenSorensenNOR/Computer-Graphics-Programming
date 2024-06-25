cmake_minimum_required(VERSION 3.18)

project(
    software_rendering
    VERSION 1.0
)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
endif()

add_subdirectory(engine)

add_executable(
    app
    main.cpp
)

target_link_libraries(app PRIVATE engine)
