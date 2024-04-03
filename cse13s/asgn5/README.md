# Assignment 5
The purpose of this assignment is to implement a Hamming code library that uses the Hamming(8,4) code. There are two programs for this assignment: one for generating Hamming codes and another for decoding them. The decoder will print statistics such as total bytes processed, uncorrected errors, corrected errors, and the error rate. There will be a program already provided that injects errors (noise) into the Hamming codes.

## Build
    $ make all

## Run
    $ ./sorting

### Command-line options
    -a: Employs all sorting algorithms.
    -b: Enables Bubble Sort.
    -s: Enables Shell Sort.
    -q: Enables Quicksort.
    -h: Enables Heapsort.
    -r <seed>: Set the random seed to <seed>. The default seed is 7092016.
    -n <size>: Set the array size to <size>. The default size is 100.
    -p <elements>: Print out the number of elements from the array. The default number of elements to print out is 100.

## Clean
    $ make clean

## Format
    $ make format
