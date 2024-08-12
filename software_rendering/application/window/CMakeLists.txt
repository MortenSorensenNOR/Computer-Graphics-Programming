cmake_minimum_required(VERSION 3.18)

add_library(window
    window.cpp
    gui.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_impl_sdl2.cpp
    imgui/imgui_impl_sdlrenderer2.cpp
    imgui/implot.cpp
    imgui/implot_items.cpp
)

target_link_libraries(window PRIVATE SDL2::SDL2 glm::glm)
