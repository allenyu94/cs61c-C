/*
 * PROJ1-1: YOUR TASK B CODE HERE
 * 
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "sparsify.h"
#include "utils.h"

/* Returns a NULL-terminated list of Row structs, each containing a NULL-terminated list of Elem structs.
 * See sparsify.h for descriptions of the Row/Elem structs.
 * Each Elem corresponds to an entry in dense_matrix whose value is not 255 (white).
 * This function can return NULL if the dense_matrix is entirely white.
 */
Row *dense_to_sparse(unsigned char *dense_matrix, int width, int height) {
	Row *sparse_matrix = NULL;   
	Row *last_row = NULL;
	Row *curr_row;
	for (int i = 0; i < height; i++) {
		curr_row = malloc(1 * sizeof(Row));
		if (curr_row == NULL) {
			allocation_failed();
		}
		curr_row->elems = NULL;
		Elem *last_elem = NULL;
		Elem *curr_elem;
		for (int k = 0; k < width; k++) {
			curr_elem = malloc(1 * sizeof(Elem));
			if (curr_elem == NULL) {
				allocation_failed();
			}
			int dense_val = dense_matrix[i * width + k];
			if (dense_val == 255) {
				free(curr_elem);
				continue;	
			} else {
				curr_elem->x = k;
				curr_elem->value = dense_matrix[i * width + k];
				curr_elem->next = NULL;
				if (last_elem == NULL) {
					curr_row->elems = curr_elem;	
				} else {
					last_elem->next = curr_elem;
				}
				last_elem = curr_elem;
				curr_elem = NULL;
			}
		}
		if (curr_row->elems == NULL) {
			free(curr_row);
			continue;
		} else {
			curr_row->y = i;
			if (last_row == NULL) {
				sparse_matrix = curr_row;
			} else {
				last_row->next = curr_row;
			}
			curr_row->next = NULL;
			last_row = curr_row;
			curr_row = NULL;
		}
	}
    return sparse_matrix;
}

/* Frees all memory associated with ELEM. */
void free_elem(Elem *elem) {
	if (elem->next == NULL) {
		free(elem);
	} else {
		free_elem(elem->next);
		free(elem);	
	}
}

/* Frees all memory associated with Row. */
void free_row(Row *row) {
	if (row->next == NULL) {
		if (row->elems != NULL) {
			free_elem(row->elems);
		}
		free(row);
	} else {
		free_row(row->next);
		if (row->elems != NULL) {
			free_elem(row->elems);
		}
		free(row);
	}
}

/* Frees all memory associated with SPARSE. SPARSE may be NULL. */
void free_sparse(Row *sparse) {
	if (sparse == NULL) {
		return;
	}
	free_row(sparse);
}
