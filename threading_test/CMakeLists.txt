cmake_minimum_required(VERSION 3.18)

project(
    thread_test
    VERSION 1.0
    LANGUAGES C
)

add_executable(
    thread_test
    main.c
)

target_link_libraries(thread_test PRIVATE)
