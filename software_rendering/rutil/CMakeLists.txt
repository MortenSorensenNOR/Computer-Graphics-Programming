add_library(rutil STATIC
    RenderUtil.c 
)

target_include_directories(rutil PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(rutil PUBLIC m display gml)
