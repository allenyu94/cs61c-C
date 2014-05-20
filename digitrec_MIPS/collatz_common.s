# CS61C Sp14 Project 1-2
# Task A: The Collatz conjecture
#
# DO NOT MODIFY THIS FILE. 

.data
spaceStr: .asciiz " "
newlineStr: .asciiz "\n"
inputStr: .asciiz "Enter the number to start the Collatz function: "
inputErrorStr: .asciiz "The number entered is invalid. Please enter a positive number.\n"
outputStr: .asciiz "\nStopping time: "
savedErrorStr: .asciiz "Warning: Value of saved registers were not preserved across function call.\n"
dneStr: .asciiz "You should not be executing common.s! Program exiting.\n"

.text
# Don't run this file!
la $a0, dneStr
li $v0, 4
syscall
li $v0, 10
syscall
	
# Prints the value in $a0, followed by a space. May be useful in debugging.
print_value:
	li $v0, 1
	syscall
	la $a0, spaceStr
	li $v0, 4
	syscall
	jr $ra


# Reads an int (>0) and returns it in $v0
read_input:
	la $a0, inputStr	# Read integer
	li $v0, 4
	syscall
	li $v0, 5
	syscall
	li $t0, 1
	slt $t1, $v0, $t0	# Check that integer is positive
	bne $t1, $0, invalid_input
	jr $ra 
invalid_input:			# If input invalid, print error msg and exit
	la $a0, inputErrorStr
	li $v0, 4
	syscall
	li $v0, 10
	syscall


# $a0 = number (>0), $a1 = function ptr
execute_function:
	# Store return address
	addiu $sp, $sp, -4
	sw $ra, 0($sp)
	# Put values in saved registers
	li $s0, -11
	li $s1, -12
	li $s2, -13
	li $s3, -14
	li $s4, -15
	li $s5, -16
	li $s6, -17
	move $s7, $sp
	
	jalr $a1		# Call function
	move $t0, $v0		# Store number of iterations in $t0
	la $a0, outputStr
	li $v0, 4
	syscall
	move $a0, $t0		# Print number of iterations
	li $v0, 1
	syscall
	la $a0, newlineStr
	li $v0, 4
	syscall
	
	# Check that values in saved registers have been preserved
	li $t0, -11
	bne $s0, $t0, saved_not_equal
	li $t0, -12
	bne $s1, $t0, saved_not_equal
	li $t0, -13
	bne $s2, $t0, saved_not_equal  
	li $t0, -14
	bne $s3, $t0, saved_not_equal
	li $t0, -15
	bne $s4, $t0, saved_not_equal
	li $t0, -16
	bne $s5, $t0, saved_not_equal
	li $t0, -17
	bne $s6, $t0, saved_not_equal
	bne $s7, $sp, saved_not_equal
	
	# Return to parent function
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	jr $ra			
saved_not_equal:
	la $a0, savedErrorStr
	li $v0, 4
	syscall
	lw $ra, 0($sp)
	addiu $sp, $sp, 4
	jr $ra

