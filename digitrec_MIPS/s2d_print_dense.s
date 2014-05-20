##### Variables #####
.data
# Header for dense matrix
head:		.asciiz	"  -----0----------1----------2----------3----------4----------5----------6----------7----------8----------9-----\n"

##### print_dense function code #####
.text
# print_dense will have 3 arguments: $a0 = address of dense matrix, $a1 = matrix width, $a2 = matrix height
print_dense:
	addiu $sp, $sp, -20
	sw $ra, 0($sp)
	sw $s0, 4($sp)
	sw $s1, 8($sp)
	sw $s2, 12($sp)
	sw $s3, 16($sp)

	move $s3, $a0 #store array address
	la $a0, head #prints the head
	jal print_str

	li $s0, 0 #current row number
	li $s1, 0 #current column/element in row

loop:
	slt $t0, $s0, $a2 #while count < max index
	beq $t0, $0, done
	move $a0, $s0
	jal print_int
	jal print_space

loop2:
	slt $t0, $s1, $a1
	bne $t0, $0, printval # if element index is less than width, print the element
	li $s1, 0
	addiu $s0, $s0, 1
	jal print_newline
	j loop

printval:
	lw $a0, 0($s3)
	jal print_intx
	jal print_space
	addiu $s3, $s3, 4
	addiu $s1, $s1, 1
	j loop2

done:
	lw $ra, 0($sp)
	lw $s0, 4($sp)
	lw $s1, 8($sp)
	lw $s2, 12($sp)
	lw $s3, 16($sp)
	addiu $sp, $sp, 20
	jr $ra
