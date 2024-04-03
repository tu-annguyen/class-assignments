#include "hamming.h"

#include "bm.h"

#include <stdio.h>

static BitMatrix *G = NULL;
static BitMatrix *H_t = NULL;

//
// Instantiates global BitMatrix G and H_t variables.
// Returns HAM_ERR if creating either bit matricies fail. Returns HAM_OK otherwise.
//
ham_rc ham_init(void) {
    G = bm_create(4, 8);

    // Check to see if creating G failed.
    if (G == NULL) {
        return HAM_ERR;
    }

    // Set bits in an identity matrix pattern for the first bm_rows(G) columns.
    for (uint32_t r = 0; r < bm_rows(G); r += 1) {
        bm_set_bit(G, r, r);
    }

    // Set bits in an inversed identity matrix pattern for the rest of G's columns.
    for (uint32_t r = 0; r < bm_rows(G); r += 1) {
        for (uint32_t c = 4; c < bm_cols(G); c += 1) {
            if (c == (4 + r)) {
                continue; // Skip setting bit at (r, 4 + r)
            }
            bm_set_bit(G, r, c);
        }
    }

    H_t = bm_create(8, 4);

    // Check to see if creating H_t failed.
    if (H_t == NULL) {
        return HAM_ERR;
    }

    // Set bits in an inversed identity matrix pattern for the first half of H_t's rows.
    for (uint32_t r = 0; r < bm_rows(H_t) / 2; r += 1) {
        for (uint32_t c = 0; c < bm_cols(H_t); c += 1) {
            if (c == r) {
                continue; // Skip setting bit at (r, c)
            }
            bm_set_bit(H_t, r, c);
        }
    }

    // Set bits in an identity matrix pattern for the rest of H_t's rows.
    uint32_t c = 0;
    for (uint32_t r = bm_rows(H_t) / 2; r < bm_rows(H_t); r += 1) {
        bm_set_bit(H_t, r, c);
        c += 1;
    }

    return HAM_OK;
}

//
// Frees allocated memory by deleting global Bitmatrix G and H_t variables.
//
void ham_destroy(void) {
    bm_delete(&G);
    bm_delete(&H_t);

    return;
}

//
// Converts an integer into a bit vector.
//
// m: The BitMatrix to store the bit vector in.
// bits: The integer to convert.
//
void i_to_bv(BitMatrix *m, uint8_t bits) {
    for (uint32_t c = 0; c < bm_cols(m); c += 1) {
        uint8_t mask = 1 << c;

        // Mask bits and store it into bit. Then shift bit to the LSB.
        uint8_t bit = bits & mask;
        bit >>= c;

        if (bit == 1) {
            bm_set_bit(m, 0, c);
        }
    }

    return;
}

//
// Performs matrix multiplication of a bit vector and bit matrix.
//
// a: The BitMatrix to be multiplied. This is the bit vector.
// b: The BitMatrix to be multiplied.
// c: The BitMatrix to store the result into.
//
void ham_vxm(BitMatrix *a, BitMatrix *b, BitMatrix *c) {
    for (uint32_t i = 0; i < bm_cols(b); i += 1) {
        uint8_t byte = bm_get_bit(c, 0, i);
        for (uint32_t j = 0; j < bm_rows(b); j += 1) {
            byte += bm_get_bit(a, 0, j) * bm_get_bit(b, j, i);
        }

        byte %= 2; // Convert byte to binary.

        // Set or clear bit of c at (0, i) according to byte.
        if (byte == 1) {
            bm_set_bit(c, 0, i);
        } else {
            bm_clr_bit(c, 0, i);
        }
    }

    return;
}

//
// Returns the binary representation of a bit vector v.
//
uint8_t bv_binary(BitMatrix *v) {
    uint8_t ans = 0;

    for (int c = bm_cols(v) - 1; c >= 0; c -= 1) {
        ans <<= 1;

        // If bit at (r, c) is 1, set 1 at LSB of ans.
        if (bm_get_bit(v, 0, c) == 1) {
            ans |= 1;
        }
    }

    return ans;
}

//
// Encodes a byte and stores it into another. Returns HAM_OK if successful and HAM_ERR otherwise.
//
// data: The byte of data to be encoded.
// code: The pointer of the variable to store the encoded data into.
//
ham_rc ham_encode(uint8_t data, uint8_t *code) {
    // If code pointer is invalid, return HAM_ERR.
    if (!code) {
        return HAM_ERR;
    }

    BitMatrix *d = bm_create(1, 4);
    BitMatrix *c = bm_create(1, 8);

    i_to_bv(d, data); // Convert data to a bit vector and store it in d.

    // Perform multiplication of bit vector d to bit matrix G and stores result in bit vector c.
    ham_vxm(d, G, c);

    // Reverse bit vector c and set it to code.
    uint8_t binary = bv_binary(c);
    *code = binary;

    // Free allocated memory for d and c.
    bm_delete(&d);
    bm_delete(&c);

    return HAM_OK;
}

//
// Decodes encoded data and stores it into a byte.
// Returns HAM_OK if there were no errors detected,
// HAM_ERR_OK if there was a correctable error detected,
// and HAM_ERR if an uncorrectable error was detected.
//
// code: The byte of encoded data to decode.
// data: The pointer of the variable to store the decoded data into.
//
ham_rc ham_decode(uint8_t code, uint8_t *data) {
    // If data pointer is invalid, return HAM_ERR.
    if (!data) {
        return HAM_ERR;
    }

    // Lookup table for error checking.
    uint32_t lookup[] = { 0, 5, 6, -1, 7, -1, -1, 4, 8, -1, -1, 3, -1, 2, 1, -1 };

    BitMatrix *d = bm_create(1, 4);
    BitMatrix *c = bm_create(1, 8);

    i_to_bv(c, code); // Convert code to a bit vector and store it in c.

    // Perform multiplication of bit vector c to bit matrix H_t and stores result in bit vector d.
    ham_vxm(c, H_t, d);

    // Reverse bit vector d and set it to index. This will be used to refer to the lookup table.
    uint8_t index = bv_binary(d);

    if (lookup[(int) index] == 0) { // If data has no error:
        // Put first 4 elements of c into d.
        for (int i = 0; i < bm_cols(d); i += 1) {
            if (bm_get_bit(c, 0, i) == 1) {
                bm_set_bit(d, 0, i);
            }
        }

        // Set data to the binary representation of d.
        uint8_t binary = bv_binary(d);
        *data = binary;

        // Free allocated memory for d and c.
        bm_delete(&d);
        bm_delete(&c);

        return HAM_OK;
    } else if (lookup[(int) index] == -1) { // If data has an uncorrectable error:
        // Free allocated memory for d and c.
        bm_delete(&d);
        bm_delete(&c);

        return HAM_ERR;
    }

    // lookup[data] > 0 meaning data has a correctable error.
    uint32_t flipped_pos = lookup[(int) data] - 1; // Position of flipped bit.

    // Flips the bit of c at (0, flipped_pos)
    if (bm_get_bit(c, 0, flipped_pos) == 1) {
        bm_clr_bit(c, 0, flipped_pos);
    } else {
        bm_set_bit(c, 0, flipped_pos);
    }

    // Re-perform multiplication of bit vector c to bit matrix H_t and stores result in bit vector d.
    ham_vxm(c, H_t, d);

    // Put first 4 elements of c (now corrected) into d.
    for (int i = 0; i < bm_cols(d); i += 1) {
        if (bm_get_bit(c, 0, i) == 1) {
            bm_set_bit(d, 0, i);
        }
    }

    // Set data to the binary representation of d.
    uint8_t binary = bv_binary(d);
    *data = binary;

    // Free allocated memory for d and c.
    bm_delete(&d);
    bm_delete(&c);

    return HAM_ERR_OK;
}
