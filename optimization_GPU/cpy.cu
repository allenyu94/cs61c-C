/*
 * Proj 3-2 SKELETON
 */

#include <float.h>
#include <stdlib.h>
#include <stdio.h>
#include <cuda_runtime.h>
#include <cutil.h>
#include "utils.h"

/* Does a horizontal flip of the array arr */
void flip_horizontal(float *arr, int width) {
	for (int i = 0; i < width; i++) {
	    for (int k = 0; k < width/2; k++) {
			swap(&arr[width * i + k], &arr[width * i + width - 1 - k]);
		}
	}	
}

/* Transposes the square array ARR. */
void transpose(float *arr, int width) {
	int offset = 1;
	for (int y = 0; y < width/2 + 1; y++) {
		for (int x = offset; x < width; x++) {
			swap(&arr[y * width + x], &arr[x * width + offset - 1]);
		}
		offset += 1;
	}
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(float *arr, int width) {
	transpose(arr, width);
	for (int y = 0; y < width/2; y++) {
		for (int x = 0; x < width; x++) {
			swap(&arr[y * width + x], &arr[width * (width - (1 + y)) + x]);
		}
	}
}

/* The kernel for the calc_min_dist function. It takes in a TEMPLATE and an IMAGE
 * and calculates the euclidean distance between the two. It then puts the answer
 * into TOTAL.
 */
__global__ void calc_min_dist_kernel(float *total, float *temp, float *image, int width) {
    int thisThreadIndex = blockIdx.x * blockDim.x + threadIdx.x;
    if (thisThreadIndex < width * width) {
        total[0] += pow(temp[thisThreadIndex] - image[thisThreadIndex], 2);
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

    float min_dist = FLT_MAX;
    float *curr_dist;
    cudaMalloc(&curr_dist, 1 * sizeof(float));
	int x_offset = i_width - t_width + 1;
	int y_offset = i_height - t_width + 1;
	int flip = 0;

    int threads_per_block = 512; // set to the max possible

    // set an initial number of blocks_per_grid
    int blocks_per_grid = ((t_width * t_width) / threads_per_block) + 1;  

	while (flip < 2) {
		for (int rotate = 0; rotate < 4; rotate++) {
			for (int y_off = 0; y_off < y_offset; y_off++) {
				for (int x_off = 0; x_off < x_offset; x_off++) {

                    printf("hello");
                    // create 3-dim vector objects to initialize values
                    dim3 dim_blocks_per_grid(blocks_per_grid, 1);
                    dim3 dim_threads_per_block(threads_per_block, 1, 1);

                    // launch kernel on GPU
                    calc_min_dist_kernel<<<dim_blocks_per_grid, dim_threads_per_block>>>(curr_dist, temp, image, t_width);

                    // wait for GPU to finish computation
                    cudaThreadSynchronize();

                    printf("a");
                    // debugging sanity.
                    CUT_CHECK_ERROR("");
                    
                    printf("b");
					if (curr_dist[0] < min_dist) {
						min_dist = curr_dist[0];
					}
					curr_dist[0] = 0;
				}
			}
			rotate_ccw_90(temp, t_width);
		}
		flip_horizontal(temp, t_width);
		flip++;
	}
    return min_dist;
}   
