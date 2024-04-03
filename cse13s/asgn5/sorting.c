#include "bubble.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "absqhr:n:p:"

// Global variables for counting each sort's statistics.
uint32_t moves = 0, compares = 0;

void print_array(uint32_t *A, int n) {
    for (int i = 0; i < n; i += 1) {
        printf("%13" PRIu32, A[i]);

        if ((i + 1) % 5 == 0) { // Start a new row after every 5 elements.
            printf("\n");
        }
    }
}

int main(int argc, char **argv) {
    // Variables for command-line options.
    int opt;
    Set opts = set_empty();
    int seed = 7092016;
    int size = 100;
    int elements = 100;

    // Loop though all inputted command-line options.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            opts = set_insert(opts, 0);
            opts = set_insert(opts, 1);
            opts = set_insert(opts, 2);
            opts = set_insert(opts, 3);
            break;
        case 'b': opts = set_insert(opts, 0); break;
        case 's': opts = set_insert(opts, 1); break;
        case 'q': opts = set_insert(opts, 2); break;
        case 'h': opts = set_insert(opts, 3); break;
        case 'r':
            seed = atoi(optarg);
            if (seed <= 0) {
                fprintf(stderr, "%s only accepts positive integers.\n", argv[0]);
                return 0;
            }
            break;
        case 'n':
            size = atoi(optarg);
            if (size <= 0) {
                fprintf(stderr, "%s only accepts positive integers.\n", argv[0]);
                return 0;
            }
            break;
        case 'p':
            elements = atoi(optarg);
            if (size <= 0) {
                fprintf(stderr, "%s only accepts positive integers.\n", argv[0]);
                return 0;
            }
            break;
        default:
            fprintf(stderr, "Usage: %s -[absqhr:n:p:].\n", argv[0]);
            return 0;
            break;
        }
    }

    // Dynamically allocate memory for arr.
    uint32_t *arr = (uint32_t *) calloc(size, sizeof(uint32_t));

    srandom(seed); // Set the random seed.

    // Create a 30 bit mask.
    uint32_t mask = 0;
    mask = ~mask << 2;
    mask >>= 2;

    // Put random numbers of 30 bits into arr.
    for (int i = 0; i < size; i += 1) {
        arr[i] = random() & mask;
    }

    uint32_t *A = (uint32_t *) calloc(size, sizeof(uint32_t));

    // Copy arr in A. A will be what's sorted.
    for (int i = 0; i < size; i += 1) {
        A[i] = arr[i];
    }

    // Run sorts for each element in opts.
    if (set_member(opts, 0)) {
        bubble_sort(A, size);

        printf("Bubble Sort\n");
        printf("%d elements, %d moves, %d compares\n", size, moves, compares);
        print_array(A, size < elements ? size : elements);

        moves = 0, compares = 0; // Reset counters.

        // Copy arr in A. A will be what's sorted.
        for (int i = 0; i < size; i += 1) {
            A[i] = arr[i];
        }
    }

    if (set_member(opts, 1)) {
        shell_sort(A, size);

        printf("Shell Sort\n");
        printf("%d elements, %d moves, %d compares\n", size, moves, compares);
        print_array(A, size < elements ? size : elements);

        moves = 0, compares = 0; // Reset counters.

        // Copy arr in A. A will be what's sorted.
        for (int i = 0; i < size; i += 1) {
            A[i] = arr[i];
        }
    }

    if (set_member(opts, 2)) {
        quick_sort(A, size);

        printf("Quick Sort\n");
        printf("%d elements, %d moves, %d compares\n", size, moves, compares);
        print_array(A, size < elements ? size : elements);

        moves = 0, compares = 0; // Reset counters.

        // Copy arr in A. A will be what's sorted.
        for (int i = 0; i < size; i += 1) {
            A[i] = arr[i];
        }
    }

    if (set_member(opts, 3)) {
        heap_sort(A, size);

        printf("Heap Sort\n");
        printf("%d elements, %d moves, %d compares\n", size, moves, compares);
        print_array(A, size < elements ? size : elements);
    }

    // Free dynamically allocated memory for arr and A.
    free(arr);
    free(A);

    return 0;
}
