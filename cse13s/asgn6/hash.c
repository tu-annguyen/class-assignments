#include "hash.h"

#include "speck.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    bool mtf;
    LinkedList **lists;
};

//
// Creates a Hashtable with size and mtf.
//
HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));

    if (ht) {
        ht->salt[0] = 0x85ae998311115ae3;
        ht->salt[1] = 0xb6fac2ae33a40089;

        ht->size = size;
        ht->mtf = mtf;

        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));

        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

//
// Frees allocated memory from ht.
//
void ht_delete(HashTable **ht) {
    for (int i = 0; i < ht_size(*ht); i += 1) {
        if ((*ht)->lists[i]) {
            ll_delete(&(*ht)->lists[i]);
        }
    }

    free((*ht)->lists);
    (*ht)->lists = NULL;

    free(*ht);
    (*ht) = NULL;

    return;
}

//
// Returns size of ht.
//
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

//
// Returns a Node pointer to the Node with oldspeak in ht. Otherwise returns NULL.
//
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
    LinkedList *ll = (LinkedList *) ht->lists[index];

    return ll_lookup(ll, oldspeak);
}

//
// Inserts oldspeak and newspeak into ht's lists.
//
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);

    if (!ht->lists[index]) {
        ht->lists[index] = ll_create(ht->mtf);
    }

    ll_insert(ht->lists[index], oldspeak, newspeak);
}

//
// Prints out ht.
//
void ht_print(HashTable *ht) {
    for (int i = 0; i < ht_size(ht); i += 1) {
        ll_print(ht->lists[i]);
    }
}
