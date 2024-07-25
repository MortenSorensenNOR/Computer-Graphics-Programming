
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(Cuda_Rasterizer LANGUAGES CXX CUDA)

set(CMAKE_CUDA_ARCHITECTURES "89")

include(CTest)

add_library(rasterizer STATIC
    rasterizer/rasterizer.h
    rasterizer/rasterizer.cu
)

target_compile_features(rasterizer PUBLIC cxx_std_11)

set_target_properties( rasterizer
    PROPERTIES CUDA_SEPARABLE_COMPILATION ON
)

add_executable(cuda_rasterizer main.cu)

set_target_properties(cuda_rasterizer PROPERTIES CUDA_SEPARABLE_COMPILATION ON)
target_link_libraries(cuda_rasterizer PRIVATE rasterizer)
