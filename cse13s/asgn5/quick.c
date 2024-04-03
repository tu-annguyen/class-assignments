#include "quick.h"

#include "stack.h"

extern uint32_t moves, compares;

int64_t partition(uint32_t *A, int64_t lo, int64_t hi) {
    uint32_t pivot = A[lo + ((hi - lo) / 2)];
    int64_t i = lo - 1;
    int64_t j = hi + 1;

    while (i < j) {
        do {
            compares += 1; // Increment compares counter.
            i += 1;
        } while (A[i] < pivot);

        do {
            compares += 1; // Increment compares counter.
            j -= 1;
        } while (A[j] > pivot);

        if (i < j) {
            // Swap A[i] and A[j].
            uint32_t temp = A[i];
            A[i] = A[j];
            A[j] = temp;

            moves += 3; // Increment moves counter.
        }
    }

    return j;
}

void quick_sort(uint32_t *A, uint32_t n) {
    uint32_t left = 0;
    uint32_t right = n - 1;

    Stack *s = stack_create();
    stack_push(s, left);
    stack_push(s, right);

    while (!stack_empty(s)) {
        int64_t hi, lo, p;

        if (!stack_pop(s, &hi)) {
            break; // Stack is empty.
        }

        if (!stack_pop(s, &lo)) {
            break; // Stack is empty.
        }

        p = partition(A, lo, hi);

        if (p + 1 < hi) {
            if (!stack_push(s, p + 1)) {
                break; // Stack failed to reallocate memory.
            }

            if (!stack_push(s, hi)) {
                break; // Stack failed to reallocate memory.
            }
        }

        if (lo < p) {
            if (!stack_push(s, lo)) {
                break; // Stack failed to reallocate memory.
            }

            if (!stack_push(s, p)) {
                break; // Stack failed to reallocate memory.
            }
        }
    }

    stack_delete(&s);

    return;
}
