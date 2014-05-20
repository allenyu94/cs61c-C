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
    #pragma omp for private(k)
	for (int i = 0; i < width; i++) {
	    for (k = 0; k < width/2; k++) {
			swap(&arr[width * i + k], &arr[width * i + width - 1 - k]);
		}
	}	
}

/* Transposes the square array ARR. */
void transpose(float *arr, int width) {
	int offset = 1;
    int b;
    #pragma omp for private(b)
	for (int a = 0; a < width; a++) {
		for (b = offset; b < width; b++) {
			swap(&arr[a * width + b], &arr[b * width + offset - 1]);
		}
		offset += 1;
	}
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(float *arr, int width) {
	transpose(arr, width);
    int p;
    #pragma omp for private(p)
	for (int j = 0; j < width/2; j++) {
		for (p = 0; p < width; p++) {
			swap(&arr[j * width + p], &arr[width * (width - (1 + j)) + p]);
		}
	}
}

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */
float calc_min_dist(float *image, int i_width, int i_height, float *template, int t_width) {
    float min_dist = FLT_MAX;
    float extra_sum;
	int x_offset = i_width - t_width + 1;
	int y_offset = i_height - t_width + 1;

    int y_off, x_off, y, x, x2;

    float *nums = malloc(sizeof(int) * 4);

    /* rotate the template 4 times. */
    for (int rotate = 0; rotate < 4; rotate++) {
        
        /* flip the template */
        for (int flip = 0; flip < 2; flip++) {

            /* translate the template */
            #pragma omp parallel for private(x_off, y_off, x, x2, y)
            for (y_off = 0; y_off < y_offset; y_off++) {
                for (x_off = 0; x_off < x_offset; x_off++) {

                    extra_sum = 0.0;

                    /* check template with this section. */

                    __m128 temp1, temp2, temp3, curr_dist;
                    for (y = 0; y < t_width; y++) {
                        for (x = 0; x < t_width; x += 4) {
                            temp1 = _mm_load_ps(&template[y * t_width + x]);
                            temp2 = _mm_load_ps(&image[(y + y_off) * i_width + (x + x_off)]);
                            temp3 = _mm_mul_ps(_mm_sub_ps(temp1, temp2), _mm_sub_ps(temp1, temp2));
                            curr_dist = _mm_add_ps(curr_dist, temp3);
                        }
                        if (t_width % 4 != 0) {
                            for (x2 = (t_width / 4) * 4; x2 < t_width; x2++) {
                                extra_sum += pow((template[y * t_width + x2] - image[(y + y_off) * i_width + (x2 + x_off)]), 2);
                            }
                        }
                    }

                    _mm_storeu_ps(nums, curr_dist);
                    float curr = 0.0;
                    for (int i = 0; i < 4; i++) {
                        curr += nums[i];
                    }
                    curr += extra_sum;

                    #pragma omp critical
                    {
                        if (curr < min_dist) {
                            min_dist = curr;
                        }
                    }

                }
            }
            flip_horizontal(template, t_width);

        }
        rotate_ccw_90(template,t_width);

    }
    free(nums);
    return min_dist;
}


