find_package(X11 REQUIRED)
find_package(OpenMP REQUIRED)

add_library(display STATIC
    Display.c    
)

target_include_directories(display PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(OpenMP_CXX_FOUND)
    target_link_libraries(my_program PRIVATE X11 Xrandr OpenMP::OpenMP_CXX)
else()
    target_link_libraries(display PRIVATE X11 Xrandr)
endif()

