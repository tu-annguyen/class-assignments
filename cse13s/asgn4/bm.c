#include "bm.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    uint8_t **mat;
};

//
// Inspired by Eugene's 2/2/2021 section.
// Calculates how many bytes to allocate given the number of bits.
//
// bits: The number of bits.
//
int bytes(int bits) {
    if (bits % 8 == 0) {
        return bits / 8;
    }
    return bits / 8 + 1;
}

//
// Code guided by Eugene's 2/2/2021 section.
// Allocates memory for a new BitMatrix.
//
// rows: The number of rows for the new BitMatrix.
// cols: The number of columns for the new BitMatrix.
//
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) calloc(1, sizeof(BitMatrix));

    if (m) { // Check to see if m was allocated successfully.
        // Instantiate m's rows and cols
        m->rows = rows;
        m->cols = cols;

        // Allocate memory for each row of mat.
        m->mat = (uint8_t **) calloc(rows, sizeof(uint8_t **));

        if (!m->mat) { // Check to see if m->mat was allocated unsuccessfully.
            return NULL;
        }

        for (uint32_t r = 0; r < rows; r += 1) {
            m->mat[r] = (uint8_t *) calloc(bytes(cols), sizeof(uint8_t));

            if (!m->mat[r]) { // Check to see if m->mat[r] was allocatted unsuccessfully.
                return NULL;
            }
        }
    } else {
        return NULL;
    }

    return m;
}

//
// Frees allocated memory in a BitMatrix m.
//
void bm_delete(BitMatrix **m) {
    for (int r = 0; r < bm_rows(*m); r += 1) {
        free((*m)->mat[r]);
        (*m)->mat[r] = NULL;
    }

    free((*m)->mat);
    (*m)->mat = NULL;

    free(*m);
    *m = NULL;

    return;
}

//
// Returns a BitMatrix m's number of rows.
//
uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

//
// Returns a BitMatrix m's number of columns.
//
uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

//
// Returns the position of the byte that contains the column in question.
//
// c: The column of the bit of interest.
//
uint32_t byte_col(uint32_t c) {
    uint32_t ans = bytes(c); // Set ans to how many bytes needed for c columns.

    // Account for zero indexing.
    if (c != 0) {
        ans -= 1;
    }

    return ans;
}

//
// Sets a bit in a BitMatrix.
//
// m: The BitMatrix to set a bit in.
// r: The row of the bit to be set.
// c: The column of the bit to be set.
//
void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t index = c % 8;
    uint8_t mask = 1 << index;
    m->mat[r][byte_col(c)] |= mask;
}

//
// Clears a bit in a BitMatrix.
//
// m: The BitMatrix to clear a bit in.
// r: The row of the bit to be cleared.
// c: The column of the bit to be cleared.
//
void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t index = c % 8;
    uint8_t mask = ~(1 << index);

    m->mat[r][byte_col(c)] &= mask;
}

//
// Inspired by Eugene's 2/2/2021 section.
// Returns the value of a specific bit.
//
// m: The BitMatrix to retrieve the value from.
// r: The row of the bit of interest.
// c: The column of the bit of interest.
//
uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint8_t index = c % 8;
    uint8_t mask = 1 << index;

    uint8_t result = (uint8_t)(m->mat[r][byte_col(c)]) & mask;

    result = result >> index;

    return result;
}

//
// Prints a BitMatrix m.
//
void bm_print(BitMatrix *m) {
    for (uint32_t r = 0; r < bm_rows(m); r += 1) {
        for (uint32_t c = 0; c < bm_cols(m); c += 1) {
            if (bm_get_bit(m, r, c) == 0) {
                printf("0");
            } else {
                printf("1");
            }
        }
        printf("\n");
    }
}
