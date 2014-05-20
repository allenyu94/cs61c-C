# CS61C Sp14 Project 1-2
# Task A: The Collatz conjecture

.globl main
.include "collatz_common.s"

main:
	jal read_input			# Get the number we wish to try the Collatz conjecture on
	move $a0, $v0			# Set $a0 to the value read
	la $a1, collatz_recursive	# Set $a1 as ptr to the function we want to execute
	jal execute_function		# Execute the function
	li $v0, 10			# Exit
	syscall
	
# --------------------- DO NOT MODIFY ANYTHING ABOVE THIS POINT ---------------------

# Returns the stopping time of the Collatz function (the number of steps taken till the number reaches one)
# using an RECURSIVE approach. This means that if the input is 1, your function should return 0.
#
# The current value is stored in $a0, and you may assume that it is a positive number.
#
# Make sure to follow all function call conventions.
collatz_recursive:
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	
	li $t0, 1
	li $v0, 0
	beq $a0, $t0, done

	li $t0, 2
	div $a0, $t0
	mfhi $t0
	bne $t0, $0, odd

even:
	mflo $a0
	j recurse

odd:
	li $t0 3
	mult $a0, $t0
	mflo $a0
	addiu $a0, $a0, 1

recurse:
	jal collatz_recursive
	addu $t1, $v0, $0
	addi $v0, $t1, 1

done:
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	jr $ra	
		
