/*
 * PROJ1-1: YOUR TASK A CODE HERE
 *
 * You MUST implement the calc_min_dist() function in this file.
 *
 * You do not need to implement/use the swap(), flip_horizontal(), transpose(), or rotate_ccw_90()
 * functions, but you may find them useful. Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "digit_rec.h"
#include "utils.h"
#include "limits.h"
#include <math.h>
#include "float.h"
#include <omp.h>
#include <nmmintrin.h>

/* Swaps the values pointed to by the pointers X and Y. */
void swap(float *x, float *y) {
	float temp = *x;
	*x = *y;
	*y = temp;
}

/* Flips the elements of a square array ARR across the y-axis. */
void flip_horizontal(float *arr, int width) {
	int k;
	#pragma omp parallel for private(k)
	for (int i = 0; i < width; i++) {
	    for (k = 0; k < width/2; k++) {
			swap(&arr[width * i + k], &arr[width * i + width - 1 - k]);
		}
	}	
}

/* Transposes the square array ARR. */
void transpose(float *arr, int width) {

    for (int y = 0; y < width - 1; y++) {
        for (int x = 0; x < width; x++) {
            if (x <= y) {
                continue;
            }

            swap(&arr[y * width + x],&arr[x * width + y]);
        }
    }
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(float *arr, int width) {
	transpose(arr, width);
	int x;
	#pragma omp parallel for private(x)
	for (int y = 0; y < width/2; y++) {
		for (x = 0; x < width; x++) {
			swap(&arr[y * width + x], &arr[width * (width - (1 + y)) + x]);
		}
	}
}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */
float calc_min_dist(float *image, int i_width, int i_height, float *template, int t_width) {
    float min_dist = FLT_MAX;
	int x_offset = i_width - t_width + 1;
	int y_offset = i_height - t_width + 1; 

	/* Flip the template. */
	for (int flip = 0; flip < 2; flip++) {

		/* rotate template 4 times. */
		for (int rotate = 0; rotate < 4; rotate++) {

			float curr_dist;
            int y_off, x_off, y, x;
			/* Translate the template around the image. */
            #pragma omp parallel for private(y_off, x_off, x, y, curr_dist) collapse(2)
            for (y_off = 0; y_off < y_offset; y_off++) {
                for (x_off = 0; x_off < x_offset; x_off++) {

                    curr_dist = 0.0;
                    float *nums = malloc(4 * sizeof(float));

                    /* Check template with this section. */
                    for (y = 0; y < t_width; y++) {

							__m128 sum_dist = _mm_setzero_ps( );
							__m128 temp = _mm_setzero_ps( );
    						__m128 imVect = _mm_setzero_ps( );
    						__m128 temVect = _mm_setzero_ps( );

                            for (x = 0; x < t_width/4*4; x += 4) {
                                imVect = _mm_loadu_ps(image + (y + y_off) * i_width + (x + x_off));
								temVect = _mm_loadu_ps(template + y * t_width + x);
								temp = _mm_sub_ps(temVect, imVect);
								temp = _mm_mul_ps(temp, temp);
								sum_dist = _mm_add_ps(sum_dist, temp);
                            }

    						_mm_storeu_ps( nums, sum_dist );

    						for (x = 0; x < 4; x++) {
        						curr_dist += nums[x];
    						} 

    						for (x = t_width/4*4; x < t_width; x++) {
								curr_dist += pow((template[y * t_width + x] - image[(y + y_off) * i_width + (x + x_off)]), 2);
							}
                    }
                    /* End check template. */

                    #pragma omp critical
                    {
                        if (curr_dist < min_dist) {
                            min_dist = curr_dist;
                        }
                    }

                    free(nums);
                }
            }
			/* End translation of template. */
			rotate_ccw_90(template, t_width);

		}
		/* End rotation of template. */
		flip_horizontal(template, t_width);
	}
    return min_dist;
}   

