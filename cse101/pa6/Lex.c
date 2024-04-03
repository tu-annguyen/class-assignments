//---------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE 101 PA6
// Lex.c 
// Top-level client and main program for PA6. 
//---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "Dictionary.h"

#define MAX_LEN 300 

int main(int argc, char **argv) {
    // Variables for command-line options.
    FILE *input = NULL, *output = NULL;

    // Set input and output files from command-line arguments.
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    
    input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "Unable to open file %s for reading.\n", argv[1]);
        return 1;
    }

    output = fopen(argv[2], "w");
    if (output == NULL) {
        fprintf(stderr, "Unable to open file %s for writing.\n", argv[2]);
        return 1;
    }

    // Count number of lines in the file.
    int numLines = 0;
    char line[MAX_LEN];
    while (fgets(line, MAX_LEN, input) != NULL) {
        numLines += 1;
    }

    fseek(input, 0, SEEK_SET); // Set file position to the beginning of the file.

    // Dynamically allocate arr.
    char **arr = (char **) calloc(numLines, sizeof(char *));
    for (int i = 0; i < numLines; i += 1) {
        arr[i] = (char *) malloc(sizeof(char));
    }
    
    // Scan strings from input file to arr array
    int i = 0;
    while (fgets(line, MAX_LEN, input) != NULL) {
        strcpy(arr[i], line);
        i += 1;
    }
    
    // Build a dictionary from arr.
    Dictionary D = newDictionary(1);
    for (int i = 0; i < numLines; i += 1) {
        insert(D, arr[i], i);
    }

    // Print Dictionary with in-order traversal to output file.
    beginForward(D);
    while (currentKey(D) != KEY_UNDEF) {
        fprintf(output, KEY_FORMAT, currentKey(D));
        next(D);
    }

    // Free allocated memory.
    for (int i = 0; i < numLines; i += 1) {
        free(arr[i]);
        arr[i] = NULL;
    }
    free(arr);
    freeDictionary(&D);
    fclose(input);
    fclose(output);

    return 0;
}