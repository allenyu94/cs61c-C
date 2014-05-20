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
#include "limits.h"
#include <math.h>
#include "float.h"


/* Swaps the values pointed to by the pointers X and Y. */
void swap(int *x, int *y) {
	int temp = *x;
	*x = *y;
	*y = temp;
}

/* Transposes the square array ARR. */
void transpose(int *arr, int width) {
    int BLOCKSIZE = 2;
    int NUMBLOCKS = width / BLOCKSIZE;
    int offset = width % BLOCKSIZE;
    int curr_off = 0;

    printf("offset %d\n", offset);
    /* Transpose all cells in a given block. */
    for (int blocks = 0; blocks < NUMBLOCKS * NUMBLOCKS; blocks++) {
        if (blocks % NUMBLOCKS == 0 && blocks != 0) {
            curr_off += offset + (BLOCKSIZE - 1 ) * width;
        }
        for (int y = 0; y < BLOCKSIZE - 1; y++) {
            for (int x = 0; x < BLOCKSIZE; x++) {
                if (x <= y) {
                    continue;
                }
                //printf("block: %d - (%d, %d)\n", blocks, arr[y * width + x + (blocks * BLOCKSIZE) + curr_off], arr[x * width + y + (blocks * BLOCKSIZE) + curr_off]);
                //printf("y: %d\n",y);
                //printf("x: %d\n", x);
                //printf("curr_ff: %d\n",curr_off);
                swap(&arr[y * width + x + (blocks * BLOCKSIZE) + curr_off], &arr[x * width + y + (blocks * BLOCKSIZE) + curr_off]);
            }
        }
    }

    /* Move the blocks to the correct spots. */
    for (int yy_b = 0; yy_b < NUMBLOCKS - 1; yy_b++) {
        for (int xx_b = 0; xx_b < NUMBLOCKS; xx_b++) {

            for (int y_b = 0; y_b < BLOCKSIZE; y_b++) {
                for (int x_b = 0; x_b < BLOCKSIZE; x_b++) {
                    if (xx_b <= yy_b) {
                        continue;
                    }
                    swap(&arr[(yy_b * BLOCKSIZE + y_b) * width + xx_b * BLOCKSIZE + x_b], &arr[(xx_b * BLOCKSIZE + y_b) * width + yy_b * BLOCKSIZE + x_b]);
                }
            }

        }
    }

    /* put the remaining in the proper place. */
    if (offset != 0) {
        printf("in offset\n");
        for (int y_ex = 0; y_ex < width - 1; y_ex++) {
            for (int x_ex = NUMBLOCKS * BLOCKSIZE; x_ex < width; x_ex++) {
                swap(&arr[y_ex * width + x_ex], &arr[x_ex * width + y_ex]);
            }
        }
    }
}

int main() {

    printf("Test\n");
    //int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
    int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25};
    //int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64};
    int width = 5;

    printf("Before Transpose\n");
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < width; x++) {
            printf("%d, ", arr[y * width + x]);
        }
        printf("\n");
    }

    printf("after transpose\n");
    transpose(arr, width);
    for (int y = 0; y < width; y++) {
        for (int x = 0; x < width; x++) {
            printf("%d, ", arr[y * width + x]);
        }
        printf("\n");
    }

    return 0;

}
