//-----------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA2
// List.c
// Implementation file for List ADT
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "List.h"


// Structs --------------------------------------------------------------------

typedef struct NodeObj {
    int data;
    struct NodeObj* next;
    struct NodeObj* prev;
} NodeObj;

typedef struct NodeObj* Node;

typedef struct ListObj {
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;
} ListObj;


// Constructors-Destructors ---------------------------------------------------

// Returns reference to new Node object. Initializes prev, next, and 
// data fields.
Node newNode(int data) {
    Node N = (Node) malloc(sizeof(NodeObj));

    if (N) {
        N->data = data;
        N->prev = NULL;
        N->next = NULL;
    
    } else {
        free(N);
        N = NULL;
    }

    return N;
}

// Frees heap memory associated with *pn and sets *pn to NULL.
void freeNode(Node *pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

// Returns reference to new empty List object. Initializes front, back, cursor, 
// length, and index fields.
List newList(void) {
    List L = (List) malloc(sizeof(ListObj));
    
    if (L) {
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->length = 0;
        L->index = -1;
    } else {
        free(L);
        L = NULL;
    }

    return L;
}

// Frees heap memory associated with *pL and sets *pL to NULL.
void freeList(List* pL) {
    if (pL != NULL && *pL != NULL) {
        while (length(*pL) != 0) {
            deleteFront(*pL);
        }

        free(*pL);
        *pL = NULL;
    }
}


// Access functions -----------------------------------------------------------

//  Returns the number of elements in L.
int length(List L) {
    return L->length;
}

// Returns index of cursor element if defined, -1 otherwise.
int index(List L) {
    if (L->cursor) {
        return L->index;
    }

    return -1;
}

// Returns front element of L.
// Pre: length(L) > 0.
int front(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling front() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling front() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    return L->front->data;
}

// Returns the back element of L.
// Pre: length(L) > 0.
int back(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling back() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling back() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    return L->back->data;
}

// Returns cursor element of L.
// Pre: length(L) > 0, index(L) >= 0.
int get(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling get() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling get() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    if (index(L) < 0) {
        fprintf(stderr, "List Error: calling get() on a List with an undefined cursor element.\n");
        exit(EXIT_FAILURE);
    }

    return L->cursor->data;
}

// Returns true (1) iff Lists A and B are in same
// state, and returns false (0) otherwise.
int equals(List A, List B) {
    if (A == NULL || B == NULL) {
        fprintf(stderr, "List Error: calling equals() on NULL List reference(s).\n");
        exit(EXIT_FAILURE);
    }

    if (length(A) != length(B)) {
        return 0;
    }

    // Reset cursors to the front for A and B.
    moveFront(A);
    moveFront(B);

    while (index(A) >= 0) {
        if (get(A) != get(B)) {
            return 0;
        }

        // Increment cursors for A and B.
        moveNext(A);
        moveNext(B);
    }

    return 1;
}


// Manipulation procedures ----------------------------------------------------

// Resets L to its original empty state.
void clear(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling clear() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }
    
    while (length(L) != 0) {
        deleteFront(L);
    }

    L->index = -1;
}

// Overwrites the cursor element’s data with x.
// Pre: length() > 0, index() >= 0
void set(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling set() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    L->cursor->data = x;
}

// If L is non-empty, sets cursor under the front element,
// otherwise does nothing.
void moveFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveFront() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) != 0) {
        L->cursor = L->front;
        L->index = 0;
    }
}

// If L is non-empty, sets cursor under the back element,
// otherwise does nothing.
void moveBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveBack() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) != 0) {
        L->cursor = L->back;
        L->index = length(L) - 1;
    }
}

// If cursor is defined and not at front, move cursor one
// step toward the front of L; if cursor is defined and at
// front, cursor becomes undefined; if cursor is undefined
// do nothing.
void movePrev(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling movePrev() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursor) {
        if (L->cursor->prev != NULL) {
            L->cursor = L->cursor->prev;
            L->index -= 1;
        
        } else {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}

// If cursor is defined and not at back, move cursor one
// step toward the back of L; if cursor is defined and at
// back, cursor becomes undefined; if cursor is undefined
// do nothing.
void moveNext(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling moveNext() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (L->cursor) {
        if (L->cursor->next != NULL) {
            L->cursor = L->cursor->next;
            L->index += 1;
        
        } else {
            L->cursor = NULL;
            L->index = -1;
        }
    }
}

// Insert new element into L. If L is non-empty,
// insertion takes place before front element.
void prepend(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling prepend() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);

    if (length(L) != 0) {
        N->next = L->front;
        L->front->prev = N;
    
    } else {
        L->back = N;
    }

    if (index(L) >= 0) {
        L->index += 1;
    }

    L->front = N;
    L->length += 1;
}

// Insert new element into L. If L is non-empty,
// insertion takes place after back element.
void append(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling append() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(x);

    if (length(L) != 0) {
        N->prev = L->back;
        L->back->next = N;
    
    } else {
        L->front = N;
    }

    L->back = N;
    L->length += 1;
}

// Insert new element before cursor.
// Pre: length() > 0, index() >= 0.
void insertBefore(List L, int x){
    if (L == NULL) {
        fprintf(stderr, "List Error: calling insertBefore() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling insertBefore() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    if(index(L) < 0) {
        fprintf(stderr, "List Error: calling insertBefore() on a List with an undefined cursor element.\n");
        exit(EXIT_FAILURE);
    }

    Node X = newNode(x);

    X->next = L->cursor;
    
    if (L->cursor->prev) {
        X->prev = L->cursor->prev;
        L->cursor->prev->next = X;
        L->cursor->prev = X;
    } else {
        L->cursor->prev = X;
        L->front = X;
    }

    L->length += 1;
    L->index += 1;
}

// Insert new element after cursor.
// Pre: length() > 0, index() >= 0.
void insertAfter(List L, int x) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling insertAfter() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling insertAfter() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    if(index(L) < 0) {
        fprintf(stderr, "List Error: calling insertAfter() on a List with an undefined cursor element.\n");
        exit(EXIT_FAILURE);
    }
    
    Node X = newNode(x);

    X->prev = L->cursor;

    if (L->cursor->next) {
        X->next = L->cursor->next;
        L->cursor->next->prev = X;
        L->cursor->next = X;
    } else {
        L->cursor->next = X;
        L->back = X;
    }

    L->length += 1;
}

// Delete the front element. 
// Pre: length() > 0.
void deleteFront(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling deleteFront() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling deleteFront() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    Node N = L->front;

    if (length(L) > 1) {
        L->front = L->front->next;
        L->front->prev = NULL;    
    } else {
        L->front = NULL;
    }

    if (index(L) >= 0) {
        L->index -= 1;
    }

    L->length -= 1;
    freeNode(&N);
}

// Delete the front element. 
// Pre: length() > 0.
void deleteBack(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling deleteBack() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling deleteBack() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    Node N = L->back;

    if (length(L) > 1) {
        L->back = L->back->prev;
        L->back->next = NULL;    
    } else {
        L->back = NULL;
    }

    if (index(L) == length(L) - 1) {
        L->index = -1;
    }

    L->length -= 1;
    freeNode(&N);
}

// Delete cursor element, making cursor undefined.
// Pre: length() > 0, index() >= 0.
void delete(List L) {
    if (L == NULL) {
        fprintf(stderr, "List Error: calling delete() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling delete() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    if(index(L) < 0) {
        fprintf(stderr, "List Error: calling get() on a List with an undefined cursor element.\n");
        exit(EXIT_FAILURE);
    }

    Node N = L->cursor;

    if (length(L) > 1) {
        if (L->cursor->prev) {
            L->cursor->prev->next = L->cursor->next;
        } else {
            L->front = L->cursor->next;
            L->cursor->next->prev = NULL; 
        }
        
        if (L->cursor->next) {
            L->cursor->next->prev = L->cursor->prev;
        } else {
            L->back = L->cursor->prev;
            L->cursor->prev->next = NULL;
        }
    }

    L->index = -1;
    L->length -= 1;
    freeNode(&N);
}


// Other operations -----------------------------------------------------------

// Prints to the file pointed to by out, a
// string representation of L consisting
// of a space separated sequence of integers,
// with front on left.
void printList(FILE* out, List L) {
    Node N = NULL;

    if (L == NULL) {
        fprintf(stderr, "List Error: calling printList() on NULL List reference.\n");
        exit(EXIT_FAILURE);
    }

    if (length(L) == 0) {
        fprintf(stderr, "List Error: calling printList() on an empty List.\n");
        exit(EXIT_FAILURE);
    }

    for (N = L->front; N != NULL; N = N->next) {
        fprintf(out, "%d", N->data);

        if (N->next != NULL) {
            fprintf(out, " ");
        }
    }
}

// Returns a new List representing the same integer
// sequence as L. The cursor in the new list is undefined,
// regardless of the state of the cursor in L. The state
// of L is unchanged.
List copyList(List L) {
    Node N = NULL;
    List L2 = newList();

    for (N = L->front; N != NULL; N = N->next) {
        append(L2, N->data);
    }

    return L2;
}