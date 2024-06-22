add_library(gml STATIC
    GML.c 
)

target_include_directories(gml PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(gml PUBLIC m)
