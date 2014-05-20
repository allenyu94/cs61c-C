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

/* Swaps the values pointed to by the pointers X and Y. */
void swap(float *x, float *y) {
	float temp = *x;
	*x = *y;
	*y = temp;
}

/* Flips the elements of a square array ARR across the y-axis. */
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
	for (int y = 0; y < (width - 1) + 1; y++) {
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

/* Returns the squared Euclidean distance between TEMPLATE and IMAGE. The size of IMAGE
 * is I_WIDTH * I_HEIGHT, while TEMPLATE is square with side length T_WIDTH. The template
 * image should be flipped, rotated, and translated across IMAGE.
 */
float calc_min_dist(float *image, int i_width, int i_height, float *template, int t_width) {
    float min_dist = FLT_MAX;
	float curr_dist;
	int x_offset = i_width - t_width + 1;
	int y_offset = i_height - t_width + 1;
	int flip = 0;

	/* Flip the template. */
	while (flip < 2) {

		/* rotate template 4 times. */
		for (int rotate = 0; rotate < 4; rotate++) {

            int y_off, x_off, y, x;
			/* Translate the template around the image. */
            #pragma omp parallel for private(x_off, y_off, x, y, curr_dist)
            for (y_off = 0; y_off < y_offset; y_off++) {
                for (x_off = 0; x_off < x_offset; x_off++) {

                    curr_dist = 0.0;

                    /* Check template with this section. */
                    for (y = 0; y < t_width; y++) {
                            for (x = 0; x < t_width; x++) {
                                curr_dist += pow((template[y * t_width + x] - image[(y + y_off) * i_width + (x + x_off)]), 2);
                            }
                    }
                    /* End check template. */

                    #pragma omp critical
                    {
                        if (curr_dist < min_dist) {
                            min_dist = curr_dist;
                        }
                        curr_dist = 0;
                    }
                }
            }
			/* End translation of template. */
			rotate_ccw_90(template, t_width);

		}
		/* End rotation of template. */
		flip_horizontal(template, t_width);
		flip++;
	}
    return min_dist;
}   

