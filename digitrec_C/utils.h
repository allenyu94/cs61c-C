/* CS61C Sp14 Project 1
 *
 * Defines structs and functions used for both parts of the project.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

typedef struct {
    unsigned char *data;
    int width;
    int height;
} Image;

// loads BMP file as a char array
Image load_bmp(char *filename);

/* Prints the image. May be helpful in debugging. */
void print_bmp(const unsigned char *data, int w, int h);

/* Should be called if memory allocation fails. */
void allocation_failed();
