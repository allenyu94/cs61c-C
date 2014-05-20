/* CS61C Sp14 Project 1
 * 
 * Contains test cases for sparsify. The test cases here are by no means 
 * complete. You are HIGHLY encouraged to write your own tests.
 *
 * ANY MODIFICATIONS TO THIS FILE WILL BE OVERWRITTEN DURING GRADING
 */

#include <stdio.h>
#include <string.h>
#include "sparsify.h"

int check_sparse(Row *sparse_matrix, int *items, int max) {
    Row *cur_row;
    Elem *cur_elem;

    int cur = 0;
    cur_row = sparse_matrix;
    while(cur_row) {
        cur_elem = cur_row->elems;
        while(cur_elem) {
            if(items[cur] != (unsigned int) cur_elem->value) return 0;
            cur_elem = cur_elem->next;
            cur++;
            if(cur>=max) return 0;
        }
        if(items[cur] != -1) return 0;
        cur_row = cur_row->next;
        cur++;
        if(cur>=max) return 0;
    }
    return items[cur]==-2 ? 1 : 0;
}


void test_sparse(char *filename, Row *sparse_matrix) {
    if(strcmp(filename, "small")==0) {
        int items[] = {1, 2, 3, -1, 4, 5, 6, -1, 7, 8, 9, -1, -2};
        printf("Result of small is %s\n", check_sparse(sparse_matrix, items, 13)==1? "correct" : "incorrect");
    } else if (strcmp(filename, "empty")==0) {
        int items[] = {-2};
        printf("Result of empty is %s\n", check_sparse(sparse_matrix, items, 1)==1? "correct" : "incorrect");
    } else if (strcmp(filename, "tiny")==0) {
        int items[] = {56, -1, -2};
        printf("Result of tiny is %s\n", check_sparse(sparse_matrix, items, 3)==1? "correct" : "incorrect");
    } else if (strcmp(filename, "sample")==0) {
		int items[] = {1, 2, -1, 3, 4, 5, -1, 6, 7, -1, -2};
		printf("Result of sample is %s\n", check_sparse(sparse_matrix, items, 11)==1? "correct" : "incorrect");
	}
}
