#include "ll.h"

#include <stdlib.h>
#include <string.h>

struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

//
// Creates a LinkedList with mtf.
//
LinkedList *ll_create(bool mtf) {
    LinkedList *l = malloc(sizeof(LinkedList));

    l->length = 0;
    l->mtf = mtf;
    l->head = node_create(NULL, NULL);
    l->tail = node_create(NULL, NULL);

    l->head->next = l->tail;
    l->tail->prev = l->head;

    return l;
}

//
// Frees allocated memory in ll.
//
void ll_delete(LinkedList **ll) {
    Node *n = (*ll)->head;

    while (n->next != NULL) {
        Node *tmp = n->next;
        node_delete(&n);
        n = tmp;
    }

    node_delete(&n);

    free(*ll);
    *ll = NULL;

    return;
}

//
// Returns the number of nodes in ll.
//
uint32_t ll_length(LinkedList *ll) {
    uint32_t count = 0;
    Node *n = ll->head->next;

    while (n->next != NULL) {
        count += 1;
        n = n->next;
    }

    return count;
}

//
// Returns the node pointer with the Node with oldspeak. Otherwise returns NULL.
//
Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    Node *n = ll->head->next;

    while (n->next != NULL) {
        if (oldspeak && strcmp(n->oldspeak, oldspeak) == 0) {
            if (ll->mtf) {
                n->next = ll->head->next;
                n->prev = ll->head;

                ll->head->next->prev = n;
                ll->head->next = n;
            }

            return n;
        }

        n = n->next;
    }

    return NULL;
}

//
// Inserts a Node with oldspeak and newspeak into ll.
//
void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    if (ll_lookup(ll, oldspeak) != NULL) {
        return;
    }

    Node *x = node_create(oldspeak, newspeak);

    x->next = ll->head->next;
    x->prev = ll->head;

    ll->head->next->prev = x;
    ll->head->next = x;

    ll->length += 1;

    return;
}

//
// Prints out ll.
//
void ll_print(LinkedList *ll) {
    if (ll) {
        Node *n = ll->head->next;

        while (n->next != NULL) {
            node_print(n);
            n = n->next;
        }
    }
    return;
}
