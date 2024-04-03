#include "bf.h"

#include "speck.h"

#include <stdlib.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

//
// Creates a BloomFilter with size.
//
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));

    if (bf) {
        bf->primary[0] = 0x02d232593fbe42ff;
        bf->primary[1] = 0x3775cfbf0794f152;

        bf->secondary[0] = 0xc1706bc17ececc04;
        bf->secondary[1] = 0xe9820aa4d2b8261a;

        bf->tertiary[0] = 0xd37b01df0ae8f8d0;
        bf->tertiary[1] = 0x911d454886ca7cf7;

        bf->filter = bv_create(size);

        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }

    return bf;
}

//
// Frees allocated memory in bf.
//
void bf_delete(BloomFilter **bf) {
    bv_delete(&(*bf)->filter);

    free(*bf);
    *bf = NULL;

    return;
}

//
// Returns the length of bf's filter.
//
uint32_t bf_length(BloomFilter *bf) {
    return bv_length(bf->filter);
}

//
// Inserts oldspeak into bf.
//
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t index_1 = hash(bf->primary, oldspeak) % bf_length(bf);
    uint32_t index_2 = hash(bf->secondary, oldspeak) % bf_length(bf);
    uint32_t index_3 = hash(bf->tertiary, oldspeak) % bf_length(bf);

    bv_set_bit(bf->filter, index_1);
    bv_set_bit(bf->filter, index_2);
    bv_set_bit(bf->filter, index_3);

    return;
}

//
// Checks to see if oldspeak is in bf.
//
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t index_1 = hash(bf->primary, oldspeak) % bf_length(bf);
    uint32_t index_2 = hash(bf->secondary, oldspeak) % bf_length(bf);
    uint32_t index_3 = hash(bf->tertiary, oldspeak) % bf_length(bf);

    return bv_get_bit(bf->filter, index_1) == 1 && bv_get_bit(bf->filter, index_2) == 1
           && bv_get_bit(bf->filter, index_3) == 1;
}

//
// Pritns bf.
//
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
