/*
 * Proj 3-2 SKELETON
 */

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <cuda_runtime.h>
#include <cutil.h>
#include "utils.h"
#include <math.h>

/* takes the square root of an INTEGER X and floors it to the nearest int. */
int floor_sqrt(int x) {
    return (int) floor(sqrt((double) x));
}

/* The kernel for horizontal flip function. */
__global__ void flip_kernel(float *arr, int width) {
    
    int N = width * width;

    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;

    int div_val = threadId / width;
    int mod_val = threadId % width;

    int f_threadId = (div_val + 1) * width - 1 - mod_val;

    if (mod_val < width / 2 && f_threadId < N) {
        float temp = arr[threadId];
        arr[threadId] = arr[f_threadId];
        arr[f_threadId] = temp;
    }
}

/* Does a horizontal flip of the array arr */
void flip_horizontal(float *arr, int width) {
    
    int N = width * width;

    int threads_per_block = 512;

    int blocks_per_grid = floor_sqrt(N / threads_per_block)+ 1;

    // create 2-D grid and 1-D block
    dim3 dim_blocks_per_grid(blocks_per_grid, blocks_per_grid);
    dim3 dim_threads_per_block(threads_per_block, 1, 1);

    // launch kernel on GPU
    flip_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(arr, width);

    cudaThreadSynchronize();
    CUT_CHECK_ERROR("");
}

/* The kernel for the transpose function. */
__global__ void transpose_kernel(float *arr, int width) {
    
    int N = width * width;

    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;

    int div_val = threadId / width;
    int mod_val = threadId % width;

    int t_threadId = mod_val * width + div_val;

    if (mod_val > div_val && t_threadId < N) {
        float temp = arr[threadId];
        arr[threadId] = arr[t_threadId];
        arr[t_threadId] = temp;
    }
}

/* Transposes the square array ARR. */
void transpose(float *arr, int width) {

    int N = width * width;

    int threads_per_block = 512;

    int blocks_per_grid = floor_sqrt(N / threads_per_block)+ 1;

    dim3 dim_blocks_per_grid(blocks_per_grid, blocks_per_grid);
    dim3 dim_threads_per_block(threads_per_block, 1, 1);

    transpose_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(arr, width);

    cudaThreadSynchronize();
    CUT_CHECK_ERROR("");
}

/* The kernel for the rotate function. */
__global__ void rotate_kernel(float *arr, int width) {
    
    int N = width * width;

    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;

    int div_val = threadId / width;
    int mod_val = threadId % width;

    int r_threadId = (width - div_val - 1) * width + mod_val;

    if (div_val < width / 2 && r_threadId < N) {
        float temp = arr[threadId];
        arr[threadId] = arr[r_threadId];
        arr[r_threadId] = temp;
    }
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(float *arr, int width) {
    
    transpose(arr, width);

    int N = width * width;

    int threads_per_block = 512;

    int blocks_per_grid = floor_sqrt(N / threads_per_block)+ 1;

    // create 2-D grid and 1-D block
    dim3 dim_blocks_per_grid(blocks_per_grid, blocks_per_grid);
    dim3 dim_threads_per_block(threads_per_block, 1, 1);

    // launch kernel on GPU
    rotate_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(arr, width);

    cudaThreadSynchronize();
    CUT_CHECK_ERROR("");


}

/* The kernel for the distance function. */
__global__ void distance_kernel(float *total, float *temp, float *image, int width, int i_width, int y_off, int x_off) {

    // size of my array
    int N = width * width;
    
    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    int threadId = blockId * blockDim.x + threadIdx.x;

    x_off += (threadId % width);
    y_off += (threadId / width);
    int offset_threadId = y_off * i_width + x_off;


    if (threadId < N) {
        total[threadId] = pow((temp[threadId] - image[offset_threadId]), 2);
    }
}

/* Stores the Euclidean distance between template and image in ARR. CPU function where
 * I call distance_kernel. Y_OFF and X_OFF decides the positioning of my TEMP on IMAGE.
 * WIDTH is the width of the template.
 */
void distance(float *arr, float *temp, float *image, int width, int i_width, int y_off, int x_off) {

    // size of my array
    int N = width * width;

    int threads_per_block = 512;

    int blocks_per_grid = floor_sqrt(N / threads_per_block)+ 1;

    // create 2-D grid and 1-D block
    dim3 dim_blocks_per_grid(blocks_per_grid, blocks_per_grid);
    dim3 dim_threads_per_block(threads_per_block, 1, 1);

    // launch kernel on GPU
    distance_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(arr, temp, image, width, i_width, y_off, x_off);

    cudaThreadSynchronize();
    CUT_CHECK_ERROR("");

}

/* The kernel for the reduce function. */
__global__ void reduce_kernel(float *arr, int N, int level) {

    int blockId = blockIdx.y * gridDim.x + blockIdx.x;
    unsigned int threadId = blockId * blockDim.x + threadIdx.x;

    if (threadId * level * 2 < N) {
        arr[threadId * (level * 2)] += arr[threadId * (level * 2) + level];
    }

}

/* Stores the sum of the Euclidean distances of each index into the first index of ARR.
 * The array size is LEN.
 */
void reduce(float *arr, int width) {

    int N = width * width;

    int threads_per_block = 512;

    int blocks_per_row = N / (threads_per_block * 2) + 1;

    int level = 1;

    while (level < N) {
        
        dim3 dimGrid(blocks_per_row, 1);
        dim3 dimBlock(threads_per_block, 1, 1);

        reduce_kernel<<<dimGrid, dimBlock>>>(arr, N, level);

        cudaThreadSynchronize();
        CUT_CHECK_ERROR("");

        level *= 2;

        blocks_per_row /= 2;
        if (blocks_per_row == 0) {
            blocks_per_row = 1;
        }
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
    // The only computation you may perform on the CPU directly derived from distance
    // values is selecting the minimum distance value given a calculated distance and a 
    // "min so far"

    // host holds the current lowest distance.
    float min_dist = FLT_MAX;
    float calc_dist = 0;

    // length of template
    int N = t_width * t_width;

    // curr_dist stores the resulting Euclidean distance of each index in GPU. 
    float *curr_dist;
    cudaMalloc(&curr_dist, N * sizeof(float));

    // calculate the offset for translation.
	int x_offset = i_width - t_width + 1;
	int y_offset = i_height - t_width + 1;

    // scan through the image
	for (int flip = 0; flip < 2; flip ++) {
		for (int rotate = 0; rotate < 4; rotate++) {
			for (int y_off = 0; y_off < y_offset; y_off++) {
				for (int x_off = 0; x_off < x_offset; x_off++) {

                    distance(curr_dist, temp, image, t_width, i_width, y_off, x_off);

                    reduce(curr_dist, t_width);

                    // copy the first index of curr_dist into calc_dist
                    cudaMemcpy(&calc_dist, curr_dist, sizeof(float), cudaMemcpyDeviceToHost);

					if (calc_dist < min_dist) {
						min_dist = calc_dist;
					}
					calc_dist = 0;
				}
			}
			rotate_ccw_90(temp, t_width);
		}
		flip_horizontal(temp, t_width);
	}
    cudaFree(curr_dist);
    return min_dist;
}
