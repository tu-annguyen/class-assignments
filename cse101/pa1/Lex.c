#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "List.h"

#define OPTIONS "i:o:"
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

    // Scan strings from input file to arr array.
    char arr[numLines][MAX_LEN];
    int i = 0;
    while (fgets(line, MAX_LEN, input) != NULL) {
        strcpy(arr[i], line);
        i += 1;
    }

    // Build an indirectly sorted list of indicies for the arr array.
    List L = newList();
    append(L, 0); // Append the first element.
    for (int i = 1; i < numLines; i += 1) {
        moveFront(L);
        bool flag = false; // Flag for if i was inserted.

        while (index(L) >= 0 && !flag) {
            if (strcmp(arr[get(L)], arr[i]) > 0) {
                insertBefore(L, i);
                flag = true;
            }

            moveNext(L);
        }

        // Append if not already inserted.
        if (!flag) { 
            append(L, i);
        }
    }

    // Construct an array with the sorted indicies in List L.
    char sortedArr[numLines][MAX_LEN];
    moveFront(L);
    i = 0;
    while(index(L) >= 0) {
        strcpy(sortedArr[i], arr[get(L)]);
        i += 1;
        moveNext(L);
    }

    // Print the sorted array to the output file.
    for (int i = 0; i < numLines; i += 1) {
        fprintf(output, sortedArr[i]);

        // Print new line character for the array element 
        // that does not have a new line character.
        if (strcmp(sortedArr[i], arr[numLines - 1]) == 0) {
            fprintf(output, "\n");
        }
    }

    fclose(input);
    fclose(output);
    freeList(&L);

    return 0;
}