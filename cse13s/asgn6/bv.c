#include "bv.h"

#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
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
// Creates a BitVector with length.
//
BitVector *bv_create(uint32_t length) {
    BitVector *b = (BitVector *) malloc(sizeof(BitVector));

    if (b) {
        b->length = length;
        b->vector = (uint8_t *) calloc(bytes(length), sizeof(uint8_t));

        if (!b->vector) {
            free(b);
            b = NULL;
        }
    }

    return b;
}

//
// Frees allocated memory in bv.
//
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    (*bv)->vector = NULL;

    free(*bv);
    *bv = NULL;

    return;
}

//
// Returns length of bv.
//
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

//
// Sets a bit in bv's vector at index i.
//
void bv_set_bit(BitVector *bv, uint32_t i) {
    uint8_t index = i % 8;
    uint8_t mask = 1 << index;

    bv->vector[i / 8] |= mask;

    return;
}

//
// Clears a bit in bv's vector at index i.
//
void bv_clr_bit(BitVector *bv, uint32_t i) {
    uint8_t index = i % 8;
    uint8_t mask = ~(1 << index);

    bv->vector[i / 8] &= mask;

    return;
}

//
// Returns the bit at bv's vector at index i.
//
uint8_t bv_get_bit(BitVector *bv, uint32_t i) {
    uint8_t index = i % 8;
    uint8_t mask = 1 << index;

    uint8_t result = (uint8_t)(bv->vector[i / 8]) & mask;

    result >>= index;

    return result;
}

//
// Prints out the bv's vector.
//
void bv_print(BitVector *bv) {
    for (int i = 0; i < bv_length(bv); i += 1) {
        if (bv_get_bit(bv, i) == 0) {
            printf("0");
        } else {
            printf("1");
        }
    }
    printf("\n");

    return;
}
