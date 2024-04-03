#include "bubble.h"

#include <stdbool.h>

extern uint32_t moves, compares;

void bubble_sort(uint32_t *A, uint32_t n) {
    bool swapped = true;

    while (swapped) {
        swapped = false;

        for (int i = 1; i < n; i += 1) {
            compares += 1; // Increment compares counter.

            if (A[i] < A[i - 1]) {
                // Swap A[i] and A[i - 1].
                uint32_t temp = A[i];
                A[i] = A[i - 1];
                A[i - 1] = temp;

                moves += 3; // Increment moves counter.

                swapped = true;
            }
        }

        n -= 1;
    }
}
