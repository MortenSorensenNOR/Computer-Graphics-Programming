cmake_minimum_required(VERSION 3.18)

project(
    software_rendering
    VERSION 1.0
)

find_package(OpenMP REQUIRED)

find_package(glm QUIET)
if (glm_FOUND)
    message(STATUS "Using glm via find_package")
endif()

if (NOT glm_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG master
        GIT_PROGRESS TRUE
    )
    message(STATUS "Using glm via FetchContent")
    FetchContent_MakeAvailable(glm)
    set_property(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_deps/glm-src" PROPERTY EXCLUDE_FROM_ALL TRUE)
endif()

find_package(SDL2 QUIET)
if (SDL2_FOUND)
    message(STATUS "Using SDL2 via find_package")
endif()

if (NOT SDL2_FOUND)
    include(FetchContent)
    set(SDL_SHARED TRUE CACHE BOOL "Build a SDL shared library (if available)")
    set(SDL_STATIC TRUE CACHE BOOL "Build a SDL static library (if available)")

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.30.x # Latest version of SDL2
        GIT_SHALLOW TRUE 
        GIT_PROGRESS TRUE 
    )
    message(STATUS "Using SDL2 via FetchContent")
    FetchContent_MakeAvailable(SDL2)
    set_property(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl2-src" PROPERTY EXCLUDE_FROM_ALL TRUE)
endif()

set(CMAKE_CXX_STANDARD 17)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og -g")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -DNDEBUG")
endif()

add_subdirectory(application)

add_executable(
    app
    main.cpp
)

target_link_libraries(app PRIVATE application glm::glm SDL2::SDL2 OpenMP::OpenMP_CXX)
