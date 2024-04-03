#include "shell.h"

#include "gaps.h"

extern uint32_t moves, compares;

void shell_sort(uint32_t *A, uint32_t n) {
    for (int g = 0; g < GAPS; g += 1) {
        for (int i = gaps[g]; i < n; i += 1) {
            uint32_t j = i;

            uint32_t temp = A[i];

            moves += 1; // Increment moves counter.

            while (1) {
                if (j < gaps[g]) {
                    break;
                }
                if (temp >= A[j - gaps[g]]) {
                    compares += 1; // Increment compares counter.
                    break;
                } else if (j >= gaps[g]) {
                    compares += 1; // Increment compares counter.
                    moves += 2; // Increment moves counter.

                    // Swap A[i] and A[j - gap].
                    A[i] = A[j - gaps[g]];
                    A[j - gaps[g]] = temp;

                    j -= gaps[g];
                }
            }

            A[j] = temp;

            moves += 1; // Increment moves counter.
        }
    }
}
