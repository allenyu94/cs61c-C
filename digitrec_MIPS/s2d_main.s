##### Include sprase matrix data  #####
.include	"s2d_sparse_matrix_data.s"

##### Variables  #####
.data
# Dense matrix to be filled in
matrix:		.word     0 : 100
# Strings
start_msg:	.asciiz	"Starting program...\n"
exit_msg:	.asciiz	"Exiting program...\n"

##### Beginning of the program  #####
.text
main:
	# start the program
	la	$a0, start_msg	# load start message
	jal	print_str	# print start message
	jal	print_newline	# print a new line

	# convert sparse matrix to dense matrix
	la	$a0, r0		# load sparse matrix data address
	la	$a1, matrix	# load dense matrix address
	li	$a2, 10
	jal	sparse2dense	# calling sparse2dense

	# print dense matrix
	la	$a0, matrix	# load dense matrix address
	li	$a1, 10		# load matrix width
	li	$a2, 10		# load matrix height
	jal	print_dense	# calling print_dense
	jal	print_newline	# print a new line

	# exit the program
	la	$a0, exit_msg	# load exit message
	jal	print_str	# print exit message
	li 	$v0, 10		# specify Exit service
	syscall			# exit!

##### Include sparse2dense and print_dense function  #####
.include	"s2d.s"
.include	"s2d_print_dense.s"

##### Helper functions  #####
print_int:
	li	$v0, 1		# specify Print Integer service
	syscall			# print integer
	jr	$ra		# return

print_intx:
	li	$v0, 34		# specify Print Integer As Hex service
	syscall			# print integer in hex
	jr	$ra		# return

print_str:
	li	$v0, 4		# specify Print String service
	syscall			# print string
	jr	$ra		# return

print_space:
	li	$a0, ' '	# load ascii value of space
	li	$v0, 11		# specify Print Character service
	syscall			# print space
	jr	$ra		# return

print_newline:
	li	$a0, '\n'	# load ascii value of newline
	li	$v0, 11		# specify Print Character service
	syscall			# print newline
	jr	$ra		# return
