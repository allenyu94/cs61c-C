##### sparse2dense function code #####
.text
# sparse2dense will have 2 arguments: $a0 = address of sparse matrix data, $a1 = address of dense matrix, $a2 = matrix width
# Recall that sparse matrix representation uses the following format:
# Row r<y> {int row#, Elem *node, Row *nextrow}
# Elem e<y><x> {int col#, int value, Elem *nextelem}
sparse2dense:
	#prologue
	addiu $sp, $sp, -16
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)

	addu $s0, $a0, $0
	addu $s1, $a1, $0

checkfirst:
	beq $a0, $0, s2ddone #if the orig sparse matrix is null, done

rowloop:
	lw $t1, 0($a0) #sets $t0 to current sparse row number
	sll $t2, $a2, 2 #4 times matrix width
	mult $t1, $t2
	mflo $t0
	addu $s1, $t0, $a1
	lw $s0, 4($a0) #get address of first elem

elemloop:
	lw $t0, 0($s0) #current column number in sparse
	sll $t0, $t0, 2
	addu $t1, $s1, $t0
	lw $t0, 4($s0) # value of elem
	sw $t0, 0($t1)	
	lw $s0, 8($s0)
	beq $s0, $0, rowcheck #this row completed
	j elemloop


rowcheck:
	lw $a0, 8($a0) #sets $t0 to next row
	beq $a0, $0, s2ddone #if next row is null finish
	j rowloop

s2ddone:		
	#epilogue
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	addiu $sp, $sp, 16
	jr $ra
	
