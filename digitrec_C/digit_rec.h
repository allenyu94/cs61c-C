/* CS61C Sp14 Project 1
 * 
 * Performs digit recognition via a pixel-by-pixel comparison of an input image
 * with template digit images. You will implement the function calc_min_dist() 
 * in the file calc_dist.c
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#define TEMPLATE_WIDTH 64

// implement this in calc_dist.c
unsigned int calc_min_dist(unsigned char *image, int i_width, int i_height, 
        unsigned char *templates, int t_width);

void check_distance(unsigned int distance, int test_id);

void check_distances(unsigned int *distances, int num_templates, char *filename);


