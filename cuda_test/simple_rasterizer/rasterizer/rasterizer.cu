#include <stdio.h>
#include "rasterizer.h"

__global__ void VecAdd(float* A, float* B, float* C, int num_elements) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < num_elements) {
        C[i] = A[i] + B[i];
    }
}
