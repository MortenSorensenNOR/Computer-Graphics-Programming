cmake_minimum_required(VERSION 3.18)

find_package(OpenMP REQUIRED)
target_link_libraries(MyTarget PUBLIC )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories("imgui")

add_library(display
    display.cpp
    gui.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/backends/imgui_impl_sdl2.cpp
    imgui/backends/imgui_impl_sdlrenderer2.cpp
)

target_link_libraries(display PRIVATE ${SDL2_LIBRARIES} OpenMP::OpenMP_CXX)
