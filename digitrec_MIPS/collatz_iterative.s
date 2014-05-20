# CS61C Sp14 Project 1-2
# Task A: The Collatz conjecture

.globl main
.include "collatz_common.s"

main:
	jal read_input			# Get the number we wish to try the Collatz conjecture on
	move $a0, $v0			# Set $a0 to the value read
	la $a1, collatz_iterative	# Set $a1 as ptr to the function we want to execute
	jal execute_function		# Execute the function
	li $v0, 10			# Exit
	syscall
	
# --------------------- DO NOT MODIFY ANYTHING ABOVE THIS POINT ---------------------

# Returns the stopping time of the Collatz function (the number of steps taken till the number reaches one)
# using an ITERATIVE approach. This means that if the input is 1, your function should return 0.
#
# The initial value is stored in $a0, and you may assume that it is a positive number.
# 
# Make sure to follow all function call conventions.
collatz_iterative:
	li $v0, 0
	li $t0, 1
	li $t1, 2

loop:
	beq	$a0, $t0, done
	div	$a0, $t1
	mfhi $t2
	bne	$t2, $0, odd

even:
	mflo $a0
	addi $v0, $v0, 1
	j loop
	
odd:
	li $t2, 3
	mult $a0, $t2
	mflo $a0
	addiu $a0, $a0, 1
	addi $v0, $v0, 1
	j loop

done:
	jr $ra
	


