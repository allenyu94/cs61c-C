.data
# Sparse matrix representation
# Row r<y> {int row#, Elem *node, Row *nextrow}
# Elem e<y><x> {int col#, int value, Elem *nextelem}
r0:		.word 0, e00, r1
e00:		.word 5, 100, e01
e01:		.word 7, 50, e02
e02:		.word 9, 33, 0

r1:		.word 1, e10, r2
e10:		.word 1, 3, 0

r2:		.word 2, e20, r3
e20:		.word 3, 244, e21
e21:		.word 8, 567, 0

r3:		.word 3, e30, 0
e30:		.word 8, 30, 0