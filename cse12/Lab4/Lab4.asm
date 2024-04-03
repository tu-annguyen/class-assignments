##########################################################################
# Created by: Nguyen, Tu-An
# tunhnguy
# 24 May 2020
#
# Assignment: Lab 4 - Sorting Integers
# CSE 12, Computer Systems and Assembly Language
# UC Santa Cruz, Spring 2020
#
# Description: This program will take in HEX arguments, convert them to 
# decimal, and sort them, meanwhile printing each step.
#
# Notes: This program is intended to be run from the MARS IDE.
##########################################################################
# Psuedocode:
# args = ["0xA0", "0x1B2", "0x0A", "0xA", "0x0C", "0x000", "0xFE"]
# ints = []
# length = len(args)
# 
# print("Program arguments:\n")
# count = 0
# while count < length:
#   print(args[count], end=" ")
#   count += 1
# 
# print("Integer values:\n")
# count = 0
# while count < length:
#   print_unsorted(count)
# count += 1
# 
# print("Sorted values:\n")
# sort_array()
# print(ints)
#
# def print_unsorted(index):
#    strHex = args[index][2:]
#    output = 0
#    for c in strHex:
#      if ord(c) >= 0x41:
#        c = chr(ord(c) - 7)
#      c = chr(ord(c) - 48)
#      output *= 16
#      output += ord(c)
#    ints.append(ans)
#    print(ans, end = " ")
#
# def sort_array():
#   swap = True
#   i = 0
#   j = 0
#   while swap is True:
#     swap = False
#     i = 0
#     j += 1
#     k = length - j
#     while i < k:
#       x = ints[i]
#       y = ints[i+1]
#       if x > y:
#         ints[i] = y
#         ints[i+1] = x
#         swap = True
#       i += 1

.text
# $s0: number of arguments
# $s1: address of pointer of arguments
# $s2: address of array
# $t0: loop counter
main:
	add $s0 $0 $a0
	la $s1 ($a1)
	la $s2 array
	addi $s2 $s2 -4				# prepare for the first iteration in print_args
	
	la $a0 args_header
	jal print_string
	addi $a1 $a1 -4				# prepare for the first iteration in print_args
	li $t0 0
	while_args:
		bge $t0 $s0 exit_args		# if $t0 >= $s0, exit while_args loop
		jal print_args
		addi $t0 $t0 1			# increments while_args loop
		j while_args
	exit_args:
	jal print_newline
	
	la $a0 unsorted_header
	jal print_string
	la $a1 ($s1)				# restore original address of pointer
	addi $a1 $a1 -4				# prepare for the first iteration in str_to_int
	li $t0 0
	while_ints:
		bge $t0 $s0 exit_ints		# if $t0 >= $s0, exit while_ints loop
		jal str_to_int
		addi $t0 $t0 1			# increments while_ints loop
		j while_ints
	exit_ints:
	jal print_newline
	
	la $a0 sorted_header
	jal print_string
	la $s2 array				# restore $s2 as address to first element of array
	jal sort_array
	for_arr:
		bge $t0 $s0 exit_arr		# if $t0 >= $s0, exit while_ints loop
		jal print_array
		addi $t0 $t0 1			# increments while_ints loop
		j for_arr
	exit_arr:
	li $a0 0xA
	jal print_char

exit_program:
	addi $v0 $0 10
	syscall
# $a0: pointer to string argument
print_args:
	addi $sp $sp -4
	sw $ra ($sp)				# store $ra
	
	addi $a1 $a1 4				# increments $a1 to address of next argument
	lw $a0 ($a1)
	jal print_string
	
	la $a0 0x20
	jal print_char				# print space
	
	lw $ra ($sp)				# load the original $ra
	addi $sp $sp 4
	jr $ra
# $a0: address of character hex digit
# $a1: pointer to string argument
# $t0: character hex digit
# $t1: hex letter offset
# $v0: int to print
str_to_int:
	addi $sp $sp -8
	sw $ra ($sp)				# store $ra
	sw $t0 4($sp)				# store $t0
	
	addi $a1 $a1 4				# increments $a1 to address of next argument
	lw $a0 ($a1)
	addi $a0 $a0 2				# start the string hex after "0x"
	lb $t0 ($a0)
	li $v0 0
	loop:
		beq $t0 $0 exit			# if $t0 == 0, exit
		
		li $t1 0
		blt $t0 0x41 hex_to_dec		# if $t0 >= 'A', $t1 = $t1 - 7
		addi $t1 $t1 -7
		
		hex_to_dec:
		addi $t0 $t0 -48
		add $t0 $t0 $t1			# $t0 = $t0 - 48 + $t1
		sll $v0 $v0 4
		add $v0 $v0 $t0			# $v0 = $v0 * 16 + $t0
		
		next_char:
		addi $a0 $a0 1			# increment address
		lb $t0 ($a0)
		j loop
	exit:
	
	addi $s2 $s2 4
	sw $v0 ($s2)
	add $a0 $0 $v0
	jal print_int				# print $v0
	
	la $a0 0x20
	jal print_char				# print space
	
	lw $ra ($sp)				# load the original $ra
	lw $t0 4($sp)				# load the original $t0
	addi $sp $sp 8
	jr $ra
# $t0: boolean value for loop
# $t1: i index
# $t2: j index
sort_array:
	addi $t0 $0 1
	addi $t1 $0 0
	addi $t2 $0 0
	
	# $t3: $s0 - j swap iteration
	while_swap:
		beq $t0 0 exit_swap		# if $t0 == false, exit
		addi $t0 $0 0			# $t0 = false
		addi $t1 $0 0			# i = 0
		addi $t2 $t2 1			# j += 1
		sub $t3 $s0 $t2
		
		# $s2: address of array
		# $t4: array[i]
		# t5: array[i+1]
		# $t6: i * 4 
		loop_arr:
			bge $t1 $t3 arr_exit	# if $t1 >= $t3, exit
			lw $t4 ($s2)		# $t4 = array[i]
			addi $s2 $s2 4		# $s2 points to next element
			lw $t5 ($s2)		# $t5 = array[i+1]
			ble $t4 $t5 no_swap	# if $t4 < $t5, skip
			sw $t4 ($s2)		# array[i+1] = $t4
			addi $s2 $s2 -4		# $s2 points to previous element
			sw $t5 ($s2)		# array[i] = $t5
			addi $t0 $0 1		# $t0 = true
			no_swap:
			addi $t1 $t1 1		# i++
			addi $t6 $0 4
			mult $t1 $t6		# $t6 = 1 * 4
			mflo $t6
			la $s2 array		# restore $s2 as address to first element of array 
			add $s2 $s2 $t6		# $s2 += i
			j loop_arr
		arr_exit:
		la $s2 array			# restore $s2 as address to first element of array 
		j while_swap
	exit_swap:
	jr $ra
print_array:
	addi $sp $sp -4
	sw $ra ($sp)				# store $ra
	
	lw $a0 ($s2)
	addi $s2 $s2 4				# increments $s2 to address of next element in array
	jal print_int
	
	la $a0 0x20
	jal print_char				# print space
	
	lw $ra ($sp)				# load the original $ra
	addi $sp $sp 4
	jr $ra	
print_newline:
	addi $sp $sp -4
	sw $ra ($sp)				# store $ra
	
	li $a0 0xA
	jal print_char
	li $a0 0xA
	jal print_char
	
	lw $ra ($sp)				# load the original $ra
	addi $sp $sp 4
	jr $ra
print_int:
	addi $v0 $0 1
	syscall
	jr $ra
print_char:
	addi $v0 $0 11
	syscall
	jr $ra
print_string:
	addi $v0 $0 4
	syscall
	jr $ra

.data
array:
	.space 32
args_header:
	.asciiz "Program arguments:\n"
unsorted_header:
	.asciiz "Integer values:\n"
sorted_header:
	.asciiz "Sorted values:\n"
