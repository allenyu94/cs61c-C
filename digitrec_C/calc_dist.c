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

/* Swaps the values pointed to by the pointers X and Y. */
void swap(unsigned char *x, unsigned char *y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

/* Flips the elements of a square array ARR across the y-axis. */
void flip_horizontal(unsigned char *arr, int width) {
	for (int i = 0; i < width; i++) {
	    for (int k = 0; k < width/2; k++) {
			swap(&arr[width * i + k], &arr[width * i + width - 1 - k]);
		}
	}	
}

/* Transposes the square array ARR. */
void transpose(unsigned char *arr, int width) {
	int offset = 1;
	for (int y = 0; y < (width - 1) + 1; y++) {
		for (int x = offset; x < width; x++) {
			swap(&arr[y * width + x], &arr[x * width + offset - 1]);
		}
		offset += 1;
	}
}

/* Rotates the square array ARR by 90 degrees counterclockwise. */
void rotate_ccw_90(unsigned char *arr, int width) {
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
unsigned int calc_min_dist(unsigned char *image, int i_width, int i_height, 
        unsigned char *template, int t_width) {
    unsigned int min_dist = INT_MAX;
	unsigned int curr_dist = 0;
	int x_offset = i_width - t_width + 1;
	int y_offset = i_height - t_width + 1;
	int flip = 0;

	/* Flip the template. */
	while (flip < 2) {

		/* rotate template 4 times. */
		for (int rotate = 0; rotate < 4; rotate++) {

			/* Translate the template around the image. */
			for (int y_off = 0; y_off < y_offset; y_off++) {
				for (int x_off = 0; x_off < x_offset; x_off++) {

					/* Check template with this section. */
					for (int y = 0; y < t_width; y++) {
						for (int x = 0; x < t_width; x++) {
							curr_dist += pow((template[y * t_width + x] - image[(y + y_off) * i_width + (x + x_off)]), 2);
						}
					}
					/* End check template. */

					if (curr_dist < min_dist) {
						min_dist = curr_dist;
					}
					curr_dist = 0;

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

