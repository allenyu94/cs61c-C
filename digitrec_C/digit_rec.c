/* CS61C Sp14 Project 1
 * 
 * Performs digit recognition via a pixel-by-pixel comparison of an input image with template digit images.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "utils.h"
#include "digit_rec.h"

/* Performs some basic tests on your code. Feel free to add additional test cases. */
void test_basic() {
    unsigned char template[] = {
        0x01, 0x02, 0x03,
        0x04, 0x05, 0x06,
        0x07, 0x08, 0x09
    };

    unsigned char translated[] = {
        0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x01, 0x02, 0x03,
        0x00, 0x00, 0x04, 0x05, 0x06,
        0x00, 0x00, 0x07, 0x08, 0x09
    };

    unsigned char rotated[] = {
        0x03, 0x06, 0x09,
        0x02, 0x05, 0x08,
        0x01, 0x04, 0x07
    };

    unsigned char flip_x_axis[] = {
        0x07, 0x08, 0x09,
        0x04, 0x05, 0x06,
        0x01, 0x02, 0x03
    };

    unsigned char flip_y_axis[] = {
        0x03, 0x02, 0x01,
        0x06, 0x05, 0x04,
        0x09, 0x08, 0x07
    };

    printf("Performing basic tests.\n");
    check_distance(calc_min_dist(template, 3, 3, template, 3), 0);
    check_distance(calc_min_dist(translated, 5, 4, template, 3), 1);
    check_distance(calc_min_dist(rotated, 3, 3, template, 3), 2);
    check_distance(calc_min_dist(flip_x_axis, 3, 3, template, 3), 3);
    check_distance(calc_min_dist(flip_y_axis, 3, 3, template, 3), 4);
}

/* Loads and tests a .BMP file. */
void test_bmp(char *img_name) {
    int num_templates, i;
    Image img;
    unsigned int *distances;
    unsigned char **templates;

    num_templates = 10;
    templates = (unsigned char **) malloc(sizeof(unsigned char *) * num_templates);
    if(!templates) allocation_failed();
    img = load_bmp("templates/0_64.bmp");
    templates[0] = img.data;
    img = load_bmp("templates/1_64.bmp");
    templates[1] = img.data;
    img = load_bmp("templates/2_64.bmp");
    templates[2] = img.data;
    img = load_bmp("templates/3_64.bmp");
    templates[3] = img.data;
    img = load_bmp("templates/4_64.bmp");
    templates[4] = img.data;
    img = load_bmp("templates/5_64.bmp");
    templates[5] = img.data;
    img = load_bmp("templates/6_64.bmp");
    templates[6] = img.data;
    img = load_bmp("templates/7_64.bmp");
    templates[7] = img.data;
    img = load_bmp("templates/8_64.bmp");
    templates[8] = img.data;
    img = load_bmp("templates/9_64.bmp");
    templates[9] = img.data;
    distances = (unsigned int *) calloc(num_templates, sizeof(unsigned int));

    for(i = 0; i < num_templates; i++) {
        distances[i] = UINT_MAX;
    }

    // load image and check:
    img = load_bmp(img_name);
    for(int i = 0; i < num_templates; i++) {
        distances[i] = calc_min_dist(img.data, img.width, img.height, templates[i], TEMPLATE_WIDTH);
    }
    check_distances(distances, num_templates, img_name);

    // free memory:
    free(img.data);
    for(i = 0; i < num_templates; i++) {
        free(templates[i]);
    }
    free(templates);
    free(distances);
}

int main(int argc, char **argv) {   
    if ( argc != 2 ) {
        test_basic();
    } else {
        test_bmp(argv[1]);
    }
    return 0;

}

