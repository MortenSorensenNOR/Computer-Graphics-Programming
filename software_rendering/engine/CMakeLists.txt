cmake_minimum_required(VERSION 3.18)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
endif()

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(glfw)
add_subdirectory(gml)
add_subdirectory(utilities)
add_subdirectory(render)

include_directories(glfw/include)

add_library(engine STATIC
    engine.c
)


target_link_libraries(engine PRIVATE glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES} gml utilities render)
