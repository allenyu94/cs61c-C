/*
 * Proj 3-2 SKELETON
 */

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <cuda_runtime.h>
#include <cutil.h>
#include "utils.h"

/* kernel for horizontal flip on GPU. */
__global__ void flip_horizontal_kernel(float *arr, int width) {
    int thisThreadIndex = blockIdx.x * blockDim.x + threadIdx.x;
    int div_val = thisThreadIndex / width;
    int mod_val = thisThreadIndex % width;

    if (mod_val < width / 2 && thisThreadIndex < (width * width)) {
        float temp = arr[thisThreadIndex];
        arr[thisThreadIndex] = arr[(div_val + 1) * width - (mod_val + 1)];
        arr[(div_val + 1) * width - (mod_val + 1)] = temp;
    }
}

/* Does a horizontal flip of the array arr */
void flip_horizontal(float *arr, int width) {
    int threads_per_block = 512;
    int blocks_per_grid = (width * width / threads_per_block) + 1;

    dim3 dim_blocks_per_grid(blocks_per_grid, 1);
    dim3 dim_threads_per_block(threads_per_block, 1, 1);

    flip_horizontal_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(arr, width);

    cudaThreadSynchronize();

    CUT_CHECK_ERROR("");
}

/* kernel for transpose on GPU. */
__global__ void transpose_kernel(float *arr, int width) {
    int thisThreadIndex = blockIdx.x * blockDim.x + threadIdx.x;
    int div_val = thisThreadIndex / width;
    int mod_val = thisThreadIndex % width;
    
    if (thisThreadIndex < (width * width) && thisThreadIndex % (width + 1) != 0) {
        float temp = arr[thisThreadIndex];
        arr[thisThreadIndex] = arr[mod_val * width + div_val];
        arr[mod_val * width + div_val] = temp;
    }

}

/* Transposes the square array ARR. */
void transpose(float *arr, int width) {
    int threads_per_block = 512;
    int blocks_per_grid = (width * width / threads_per_block) + 1;

    dim3 dim_blocks_per_grid(blocks_per_grid, 1);
    dim3 dim_threads_per_block(threads_per_block, 1, 1);

    transpose_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(arr, width);

    cudaThreadSynchronize();

    CUT_CHECK_ERROR("");
}

/* kernel for rotation on GPU. */
__global__ void rotate_kernel(float *arr, int width) {
    int thisThreadIndex = blockIdx.x * blockDim.x + threadIdx.x;
    int div_val = thisThreadIndex / width;
    int mod_val = thisThreadIndex % width;
    
    if (thisThreadIndex < (width * width) && div_val > (width / 2)) {
        float temp = arr[thisThreadIndex];
        arr[thisThreadIndex] = arr[(width - div_val - 1) * width + mod_val];
        arr[(width - div_val - 1) * width + mod_val] = temp;
    }
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(float *arr, int width) {
    int threads_per_block = 512;
    int blocks_per_grid = (width * width / threads_per_block) + 1;

    dim3 dim_blocks_per_grid(blocks_per_grid, 1);
    dim3 dim_threads_per_block(threads_per_block, 1, 1);

    rotate_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(arr, width);

    cudaThreadSynchronize();

    CUT_CHECK_ERROR("");
}

/* The kernel for the calc_min_dist function. It takes in a TEMPLATE and an IMAGE
 * and calculates the euclidean distance between the two. It then puts the answer
 * into TOTAL.
 */
__global__ void calc_min_dist_kernel(float *total, float *temp, float *image, int width) {
    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;

    if (threadId < (width * width)) {
        total[threadId] = pow((temp[threadId] - image[threadId]), 2);
    }
}

__global__ void add_distance_kernel(float *arr, int len, int level) {
    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;

    if (threadId * level * 2 < len) {
        arr[threadId * level * 2] += arr[threadId * level * 2 + level];
    }

}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */
float calc_min_dist(float *image, int i_width, int i_height, float *temp, int t_width) {
    // float* image and float* temp are pointers to GPU addressible memory
    // You MAY NOT copy this data back to CPU addressible memory and you MAY 
    // NOT perform any computation using values from image or temp on the CPU.

    int len = t_width * t_width;

    // host copies of min_dist, curr_dist_temp
    float min_dist = FLT_MAX;
    //float *curr_dist_temp;
    //curr_dist_temp = (float *) malloc(len * sizeof(float));

    // device copies of curr_dist
    float *curr_dist;
    cudaMalloc(&curr_dist, len * sizeof(float));

    // instantiating the grid and block dimensions to 2D Grid and 1D block
    int threads_per_block = 512;

    int blocks_per_grid = (t_width / threads_per_block) + 1; 

    dim3 dim_blocks_per_grid(blocks_per_grid, blocks_per_grid); // gridDim.x = blocks_per_grid_x and gridDim.y = blocks_per_grid_y
    dim3 dim_threads_per_block(threads_per_block, 1, 1); // blockDim.x = threads_per_block

    // Launch calc_min_dist_kernel on GPU
    calc_min_dist_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(curr_dist, temp, image, t_width);

    // Wait for GPU to finish computation
    cudaThreadSynchronize();

    CUT_CHECK_ERROR("");

    // Copy result back to host
    // cudaMemcpy(curr_dist_temp, curr_dist, t_width * t_width * sizeof(float), cudaMemcpyDeviceToHost);

    
    int level = 1;
    while (level != len) {
        
        int threads_per_block2 = 512;

        int blocks_per_grid2 = (t_width / threads_per_block2) + 1; 
    
        dim3 dim_blocks_per_grid2(blocks_per_grid2, blocks_per_grid2); // gridDim.x = blocks_per_grid_x and gridDim.y = blocks_per_grid_y
        dim3 dim_threads_per_block2(threads_per_block, 1, 1); // blockDim.x = threads_per_block

        add_distance_kernel<<<dim_blocks_per_grid2, dim_threads_per_block2>>>(curr_dist, t_width * t_width, level);

        cudaThreadSynchronize();

        CUT_CHECK_ERROR("");

        level *= 2;

        blocks_per_grid2 = ((sqrt(len / 2) + 1) / threads_per_block2);
        if (blocks_per_grid2 == 0) {
            blocks_per_grid2 = 1;
        }
        
    } 

    float curr_min = 0.0;

    cudaMemcpy(&curr_min, curr_dist, sizeof(float), cudaMemcpyDeviceToHost);

    if (curr_min < min_dist) {
        min_dist = curr_min;
    }

    // Cleanup
    cudaFree(curr_dist);

    return min_dist;
    
}

