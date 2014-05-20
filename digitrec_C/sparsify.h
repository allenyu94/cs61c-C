/* CS61C Sp14 Project 1
 * 
 * Converts a bitmap array into a sparse representation of the matrix. You will implement
 * the functions dense_to_sparse() and free_sparse() located in the file make_sparse.c.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

/* A struct representing an element of the image. X refers to the element's x position.
 * VALUE is the value of the pixel. NEXT points to the next element in the linked list,
 * or NULL if there are no other items. A linked list of Elem structs should be sorted 
 * by X from smallest to largest. */
typedef struct Elem {
    int x;
    unsigned char value;
    struct Elem *next;
} Elem;

/* A struct representing a row of the image. Y refers to the element's y position.
 * ELEMS points to a linked list of Elem structs. NEXT points to the next row in
 * the linked list, or NULL if there are no more items. A linked list of Row
 * structs should be sorted by Y from smallest to largest.
 */
typedef struct Row {
    int y;
    Elem *elems;
    struct Row *next;
} Row;

/* Implement this in make_sparse.c */
Row *dense_to_sparse(unsigned char *dense_matrix, int width, int height);

/* Implement this in make_sparse.c */
void free_sparse(Row *sparse_matrix);

/* You may find this function helpful in debugging */
void print_sparse(Row *sparse_matrix);

void test_sparse(char *filename, Row *sparse_matrix);
