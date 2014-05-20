/* CS61C Sp14 Project 1
 * 
 * Contains test cases for digit-rec. The test cases here are by no means 
 * complete. You are HIGHLY encouraged to write your own tests.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <stdio.h>

void check_distance(unsigned int distance, int test_id) {
    switch(test_id) {
        case 0: printf("Testing with self...%s\n", distance==0?"passed":"failed"); break;
        case 1: printf("Testing with translated image...%s\n", distance==0?"passed":"failed"); break;
        case 2: printf("Testing with rotated image...%s\n", distance==0?"passed":"failed"); break;
        case 3: printf("Testing with image flipped across x-axis...%s\n", distance==0?"passed":"failed"); break;
        case 4: printf("Testing with image flipped across y-axis...%s\n", distance==0?"passed":"failed"); break;
        default: printf("Unrecongized test"); return;
    }
}

/* Finds the minimal distance in DISTANCES (of size NUM_TEMPLATES) and declares the corresponding digit as the classification result for the file FILENAME.
 */
void check_distances(unsigned int *distances, int num_templates, char *filename) {
    int i, best, best_ind;
    unsigned int dist;
    printf("Checking student results for input: %s\n", filename);
    for(i = 0; i < num_templates; i++) {
        dist = distances[i];
        printf("%d: %u\n", i, dist);
        if (i == 0 || dist < best) {
            best = dist;
            best_ind = i;
        }
    }
    printf("I think your image is a %d\n", best_ind);
}

