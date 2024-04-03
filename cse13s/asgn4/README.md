# Assignment 4
The purpose of this assignment is to implement  a Hamming code library that uses the Hamming(8,4) code. There are two programs for this assignment: one for generating Hamming codes and another for decoding them. The decoder will print statistics such as total bytes processed, uncorrected errors, corrected errors, and the error rate. There will be a program already provided that injects errors (noise) into the Hamming codes.

## Build
    $ make all

## Run generator
    $ ./gen

### Command-line options
    -i <input>: Specify the input file to read. The default input file is stdin.
    -o <output>: Specify the output file to print to. The default output file is stdout.

## Run error injector
    $ ./err

### Command-line options
    -e <rate>: Set the rate at which the program inject errors. The input is 0.001 by default and must be within [0.0, 1.0].
    -s <seed>: Set the random seed. The input is 2020 by default and must be a positive integer.

## Run decoder
    $ ./dec

### Command-line options
    -i <input>: Specify the input file to read. The default input file is stdin.
    -o <output>: Specify the output file to print to. The default output file is stdout.

## Clean
    $ make clean

## Format
    $ make format
