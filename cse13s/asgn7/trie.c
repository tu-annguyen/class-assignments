#include "trie.h"

#include "code.h"

#include <stdlib.h>

//
// Initialize a TrieNode. Set the TrieNode's code. Set each of the TrieNode's
// children are NULL. Return a pointer to the node.
//
// code: The code to set.
//
TrieNode *trie_node_create(uint16_t code) {
    TrieNode *tn = malloc(sizeof(TrieNode));

    if (tn) {
        tn->code = code;

        for (int i = 0; i < ALPHABET; i += 1) {
            tn->children[i] = NULL;
        }

    } else {
        free(tn);
        tn = NULL;
    }

    return tn;
}

//
// Free memory allocated for a TrieNode.
//
// n: The TrieNode of interest.
//
void trie_node_delete(TrieNode *n) {
    free(n);
    n = NULL;
}

//
// Initialize a trie with a root TrieNode. The root has EMPTY_CODE as its code.
// Return a pointer to the root.
//
TrieNode *trie_create(void) {
    TrieNode *root = trie_node_create(EMPTY_CODE);

    if (!root) { // If creating the root was unsuccessful.
        trie_node_delete(root);

        free(root);
        root = NULL;
    }

    return root;
}

//
// Code inspired by Eugene's 3/2/2021 section.
// Reset a trie to just the root TrieNode.
//
// root: The pointer to the root of the trie to reset.
//
void trie_reset(TrieNode *root) {
    for (int i = 0; i < ALPHABET; i += 1) {
        if (root->children[i]) {
            trie_delete(root->children[i]);
        }
    }
}

//
// Code inspired by Eugene's 3/2/2021 section.
// Delete a sub-trie starting from the trie rooted at TrieNode n.
//
void trie_delete(TrieNode *n) {
    for (int i = 0; i < ALPHABET; i += 1) {
        if (n->children[i]) {
            trie_delete(n->children[i]);
            n->children[i] = NULL;
        }
    }

    trie_node_delete(n);
}

//
// Return a pointer to the child node representing the symbol sym.
// If the symbol doesn't exist, NULL is returned.
//
TrieNode *trie_step(TrieNode *n, uint8_t sym) {
    return n->children[sym];
}
