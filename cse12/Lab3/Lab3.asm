##########################################################################
# Created by: Nguyen, Tu-An
# tunhnguy
# 12 May 2020
#
# Assignment: Lab 3 - ASCII-risks
# CSE 12, Computer Systems and Assembly Language
# UC Santa Cruz, Spring 2020
#
# Description: This program will print out a pattern with numbers and stars (asterisks).
#
# Notes: This program is intended to be run from the MARS IDE.
##########################################################################
# Psuedocode:
# height = user input data
# numInts = 1
# intsToPrint = 1
# numAsts = 2 * (height - 1)
# line = ""
# 
# for x in range(height):
#   for y in range(numInts):
#     line += str(intsToPrint) + "\t"
#   for z in range(numAsts):
#     line += "*\t"
#   for w in range(numInts):
#     if w is numInts - 1:
#       line += str(intsToPrint - 1)
#     else:
#       line += str(intsToPrint - 1) + "\t"
#     intsToPrint -= 1
#   numInts += 1
#   intsToPrint += numInts - 1
#   numAsts -= 2
#   print(line)
#   print("\n")
#   line = ""

.text
# Register usage:
# $s0: user inputted height
# $s1: number of integers to be printed before and after the asterisks
# $s2: integers to be printed out before and after the asterisks
# $s3: number of asterisks to be printed out during each line
main:
	la   $a0, request_height		# Prompts the user to imput the height
	jal  print_string
	
	addi $v0, $0,  5			# Gets the user-inputted height and stores it into $s0
	syscall
	add  $s0, $0,  $v0
	
	blez $s0, print_error			# If height <= 0, print the error message
	
	add  $a0, $0,  0xA			# Prints new line
	jal print_char
	
	# Instantiates the main variables of the program
	addi $s1, $0,  1
	addi $s2, $0,  1
	subi $s3, $s0, 1
	add  $s3, $s3, $s3
	
	# Prints an entire line, including the numbers and asterisks
	# $t0: loop counter
	addi $t0, $0,  0
	while:
		bge  $t0, $s0, exit
		
		# Prints the integers before the asterisks
		# $t1: loop counter
		addi $t1, $0,  0
		while_ints:
			bge  $t1, $s1, exit_ints
			jal  print_ints

			addi $t1, $t1, 1	# Increments the while_ints loop
			j    while_ints
		exit_ints:
		
		# Prints the asterisks
		# $t2: loop counter
		addi $t2, $0, 0
		while_asts:
			bge  $t2, $s3, exit_asts
			jal  print_asts
			
			addi $t2, $t2, 1	# Increments the while_asts loop
			j    while_asts
		exit_asts:
		
		# Prints the integers backwards after the asterisks
		# $t3: loop counter
		addi $t3, $0, 0
		while_ints_backw:
			bge  $t3, $s1, exit_ints_backw
			jal  print_ints_backw			
			
			addi $t3, $t3, 1	# Increments the while_ints_backw loop
			j    while_ints_backw
		exit_ints_backw:
		
		# Updates $s1-3 accordingly
		# $t6: number of integers to print - 1
		addi $s1, $s1, 1
		subi $t6, $s1, 1
		add  $s2, $s2, $t6
		subi $s3, $s3, 2
		
		add  $a0, $0,  0xA		# Prints new line
		jal  print_char
		
		addi $t0, $t0, 1		# Increments the while loop
		j    while
	exit:
exit_program:
	addi $v0, $0,  10
	syscall
# Register usage:
# $t5: return address
print_ints:
	add  $t5, $0,  $ra			# stores the jal print_ints $ra into $t5 because $ra gets overwritten by jal print_int and print_char

	add  $a0, $0,  $s2			# Prints the integer
	jal  print_int
	
	addi $a0, $0,  0x9			# Prints a tab
	jal  print_char
	
	addi $s2, $s2, 1			# Increments $s2 to the next integer to be printed
	jr   $t5
# Register usage:
# $t5: return address
print_asts:
	add  $t5, $0,  $ra
	
	add  $a0, $0,  0x2A			# Prints an asterisk
	jal  print_char
	
	addi $a0, $0,  0x9			# Prints a tab
	jal  print_char
	
	jr   $t5
# Register usage:
# $t4: next integer to be forward printed
# $t5: return address
# $t7: number of the last iteration of the while_ints_backw loop
print_ints_backw:
	add  $t5, $0,  $ra
	
	add  $t4, $0,  $s2
	subi $t4, $t4, 1
	
	add  $a0, $0,  $t4
	jal  print_int
	
	# Determines whether or not to print a tab. If it's the last number of the line, refrains from printing the tab
	subi $t7, $s1, 1
	bne  $t3, $t7, print_tab
	
	continue:
		subi $s2, $s2, 1		# Sets $s2 to the correct next integer to be printed to account for the above subtraction
		jr   $t5
print_tab:
	addi $a0, $0,  0x9			# Prints a tab
	jal  print_char
	j    continue
print_error:
	la   $a0, error_msg
	jal  print_string
	j    main
print_int:
	addi $v0, $0,  1
	syscall
	jr   $ra
print_char:
	addi $v0, $0,  11
	syscall
	jr   $ra
print_string:
	addi $v0, $0,  4
	syscall
	jr   $ra

.data
request_height:
	.asciiz "Enter the height of the pattern(must be greater than 0):\t"
error_msg:
	.asciiz "Invalid Entry!\n"