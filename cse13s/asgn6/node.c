#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Returns a pointer to a string copy.
//
// c: String to copy.
//
char *str_copy(char *c) {
    char *cp = malloc(strlen(c) + 1);

    for (int i = 0; i < strlen(c) + 1; i += 1) {
        cp[i] = c[i];
    }

    return cp;
}

//
// Creates a node with oldspeak and newspeak.
//
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = malloc(sizeof(Node));

    if (oldspeak) {
        n->oldspeak = str_copy(oldspeak);
    } else {
        n->oldspeak = NULL;
    }

    if (newspeak) {
        n->newspeak = str_copy(newspeak);
    } else {
        n->newspeak = NULL;
    }

    n->next = NULL;
    n->prev = NULL;

    return n;
}

//
// Frees allocated memory of a string cp.
//
void str_copy_delete(char **cp) {
    free(*cp);
    *cp = NULL;

    return;
}

//
// Frees allocated memory a Node n.
//
void node_delete(Node **n) {
    str_copy_delete(&(*n)->oldspeak);
    str_copy_delete(&(*n)->newspeak);

    free(*n);
    *n = NULL;

    return;
}

//
// Prints a Node n.
//
void node_print(Node *n) {
    if (n) {
        if (n->newspeak != NULL && n->oldspeak != NULL) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        }

        if (n->newspeak == NULL && n->oldspeak != NULL) {
            printf("%s\n", n->oldspeak);
        }
    }

    return;
}
