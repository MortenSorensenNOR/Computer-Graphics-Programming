find_package(X11 REQUIRED)

add_library(display STATIC
    Display.c    
)

target_include_directories(display PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(display PUBLIC X11 Xrandr)
