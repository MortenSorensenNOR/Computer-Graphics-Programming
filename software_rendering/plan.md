# Software overview:

## Project file structure:

project/
│
├── src/
│   └── main.cpp
│
├── Application/
│   ├── Application.h
│   ├── Application.cpp
│   │
│   ├── Core_Utilities/
│   │   └── buffer.h
│   │
│   ├── Engine/
│   │   ├── engine.h
│   │   ├── engine.cpp
│   │   │
│   │   ├── Scene/
│   │   ├── Renderer/
│   │   └── Game_logic/
│   │
│   └── Window/
│       ├── window.h
│       ├── window.cpp
│       ├── gui.h
│       ├── gui.cpp
│       ├── implot/
│       └── imgui/
│
└── CMakeLists.txt <-- Handels fetching of required dependencies using fetchcontent 


# TODO:
## Implement tiled rendering
1. Transforme vertexes to view space and clip triangles
2. Bin triangles into tiles and run rasterization and fragment shaders on each tile

Nr. 1 is easy. Nr. 2 requires reworking the math from simple_renderer.
A size of 32x32 pixels for each tile has been choosen. A smaller size of 16x16 may produce better results on cpu.
Each tile will be rendered in 2x2 pixel chunks. This is the smallest chunk that enables mip-mapping. On GPU this would likely 
be done by clustering 4-by-4 threads together to compute each chunk.
