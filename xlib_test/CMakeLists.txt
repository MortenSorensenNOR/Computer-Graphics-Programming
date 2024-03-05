cmake_minimum_required(VERSION 3.18)

project(
    GPU_Emulator
    VERSION 1.0
    LANGUAGES C
)

add_executable(
    Emulate
    main.c
    GAEDisplay.c
)

target_link_libraries(Emulate PRIVATE X11)
