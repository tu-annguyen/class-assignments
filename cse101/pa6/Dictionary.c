//-----------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE 101 PA6
// Dictionary.c 
// Implementation file for Dictionary ADT
//-----------------------------------------------------------------------------

#include "Dictionary.h"

// Structs --------------------------------------------------------------------

typedef struct NodeObj {
    KEY_TYPE key;
    VAL_TYPE val;
    struct NodeObj* left;
    struct NodeObj* right;
    struct NodeObj* parent;
} NodeObj;

typedef struct NodeObj* Node;

typedef struct DictionaryObj {
    int unique;
    Node root;
    int size;
    Node current;
    Node nil;
} DictionaryObj;

// Constructors-Destructors ---------------------------------------------------

// newNode()
// Creates a new empty Node object. Initializes key and val data fields.
Node newNode(KEY_TYPE key, VAL_TYPE val) {
    Node N = (Node) malloc(sizeof(NodeObj));

    N->key = key;
    N->val = val;
    N->left = NULL;
    N->right = NULL;
    N->parent = NULL;

    return N;
}

// freNode()
// Frees heap memory associated with *pN and sets *pN to NULL.
void freeNode(Node *pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }

    return;
}

// newDictionary()
// Creates a new empty Dictionary. If unique==false (0), then the Dictionary 
// will accept duplicate keys, i.e. distinct pairs with identical keys. If 
// unique==true (1 or any non-zero value), then duplicate keys will not be 
// accepted. In this case, the operation insert(D, k) will enforce the 
// precondition: lookup(D, k)==VAL_UNDEF
Dictionary newDictionary(int unique) {
    Dictionary D = (Dictionary) malloc(sizeof(DictionaryObj));

    D->root = NULL;
    D->size = 0;
    D->current = NULL;

    // Create nil Node.
    Node nil = newNode(KEY_UNDEF, VAL_UNDEF);
    D->nil = nil;

    return D;
}

// freeDictionary()
// Frees heap memory associated with *pD, sets *pD to NULL.
void freeDictionary(Dictionary* pD) {
    if (pD != NULL && *pD != NULL) {
        makeEmpty(*pD);
        freeNode(&(*pD)->nil);
        free(*pD);
        *pD = NULL;
    }
    
    return;
}


// Access functions -----------------------------------------------------------

// size()
// Returns the number of (key, value) pairs in Dictionary D.
int size(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling size() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    return D->size;
}

// getUnique()
// Returns true (1) if D requires that all pairs have unique keys. Returns
// false (0) if D accepts distinct pairs with identical keys.
int getUnique(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling getUnique() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    return D->unique;
}

// lookupNode()
// Helper function for lookup().
// Returns a Node of the matching key k, if it exists. Returns NULL if the
// key does not exist in the tree with root Node N.
Node lookupNode(Node N, KEY_TYPE k) {
    while (N != NULL) {
        // Pass right subtree as new tree.
        if (KEY_CMP(k, N->key) > 0) {
            N = N->right;
        
        // Pass left subtree as new tree.
        } else if (KEY_CMP(k, N->key) < 0) {
            N = N->left;

        } else {
            return N; // If the key is found.
        }
    }

    return NULL;
}

// lookup()
// If Dictionary D contains a (key, value) pair whose key matches k (i.e. if
// KEY_CMP(key, k)==0), then returns value. If D contains no such pair, then
// returns VAL_UNDEF.
VAL_TYPE lookup(Dictionary D, KEY_TYPE k) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling lookup() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    Node N = lookupNode(D->root, k);

    if (N != NULL) {
        return N->val;
    }

    return VAL_UNDEF;
}


// Manipulation procedures ----------------------------------------------------

// treeMin()
// Returns the Node with the smallest key in a tree. The iteration starts with
// Node N.
// Pre: N != NULL.
Node treeMin(Node N) {
    if (N == NULL) {
        fprintf(stderr, "Dictionary Error: calling treeMin() with a NULL Node.\n");
        exit(EXIT_FAILURE);
    }

    while (N->left != NULL) {
        N = N->left;
    }

    return N;
}

// treeMax()
// Returns the node with the largest key in a tree with Node N as the root of
// the tree.
// Pre: N != NULL.
Node treeMax(Node N) {
    if (N == NULL) {
        fprintf(stderr, "Dictionary Error: calling treeMax() with a NULL Node.\n");
        exit(EXIT_FAILURE);
    }

    while (N->right != NULL) {
        N = N->right;
    }

    return N;
}

// treeSuccessor()
// If there is a successor node of Node N, returns the successor node.
// Else, returns the predecessor node of N.
Node treeSuccessor(Dictionary D, Node N) {
    if (N->right != NULL) {
        return treeMin(N->right);
    }

    Node P = N->parent;
    while (P != NULL && P->right != NULL && KEY_CMP(N->key, P->right->key) == 0) {
        N = P;
        P = P->parent;
    }

    if (P == NULL) { // Make P the NIL Node if NULL.
        P = D->nil;
    }
    return P;
}

// treePredecessor()
// If there is a predecessor node of Node N, returns the predecessor
// node. Else, returns the successor node of N.
Node treePredecessor(Dictionary D, Node N) {
    if (N->left != NULL) {
        return treeMax(N->left);
    }

    Node P = N->parent;
    while (P != NULL && P->left != NULL && KEY_CMP(N->key, P->left->key) == 0) {
        N = P;
        P = P->parent;
    }

    if (P == NULL) { // Make P the NIL Node if NULL.
        P = D->nil;
    }
    return P;
}

// insert()
// Insert the pair (k,v) into Dictionary D. 
// If getUnique(D) is false (0), then there are no preconditions.
// If getUnique(D) is true (1), then the precondition lookup(D, k)==VAL_UNDEF
// is enforced. 
void insert(Dictionary D, KEY_TYPE k, VAL_TYPE v) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling insert() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    if (getUnique(D) == 1 && lookup(D, k) != VAL_UNDEF) {
        fprintf(stderr, "Dictionary Error: calling insert() with a duplicate key.\n");
        exit(EXIT_FAILURE);
    }

    Node N = newNode(k, v);

    Node cursor = D->root;
    Node P = NULL;

    // Iterate through D using Node cursor. Node P will be Node N's
    // parent once the loop exits. 
    while (cursor != NULL) {
        P = cursor;

        if (KEY_CMP(N->key, cursor->key) < 0) {
            cursor = cursor->left;

        } else {
            cursor = cursor->right;
        }
    }

    N->parent = P;

    // Insert Node N into D.
    if (size(D) == 0) { // Dictionary is empty.
        D->root = N;
    
    } else if (KEY_CMP(k, P->key) < 0) {
        P->left = N;
    
    } else {
        P->right = N;
    }

    D->size += 1;
    return;
}

// transplant()
// Helper function for delete(). Replaces Node U with Node V in the BST.
// Adjusts the other nodes accordingly.
void transplant(Dictionary D, Node U, Node V) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling transplant() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    if (U->parent == NULL) {
        D->root = V;

    } else if (U != NULL && U->parent->left != NULL && KEY_CMP(U->key, U->parent->left->key) == 0) {
        U->parent->left = V;

    } else {
        U->parent->right = V;
    }

    if (V != NULL) {
        V->parent = U->parent;
    }
}

// delete()
// Remove the pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary D, KEY_TYPE k) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling delete() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    Node Z = lookupNode(D->root, k);

    if (Z == NULL) {
        fprintf(stderr, "Dictionary Error: calling delete() on non-existant key.\n");
        exit(EXIT_FAILURE);
    }

    if (Z->left == NULL) {
        transplant(D, Z, Z->right);
    
    } else if (Z->right == NULL) {
        transplant(D, Z, Z->left);

    } else {
        Node Y = treeMin(Z->right);
        if (KEY_CMP(Y->parent->key, Z->key) != 0) {
            transplant(D, Y, Y->right);
            Y->right = Z->right;
            Y->right->parent = Y;
        }

        transplant(D, Z, Y);
        Y->left = Z->left;
        Y->left->parent = Y;
    }

    // Set D->current to the NIL node if is equal to Z. Then, delete Z.
    if (D->current != NULL && D->current->key != KEY_UNDEF && KEY_CMP(D->current->key, Z->key) == 0) {
        D->current = D->nil;
    }
    freeNode(&Z);
    
    D->size -= 1;
    return;
}

// makeEmpty()
// Reset Dictionary D to the empty state, containing no pairs.
void makeEmpty(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling makeEmpty() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    while (size(D) != 0) {
        delete(D, D->root->key);
    }

    D->current = NULL;

    return;
}

// beginForward()
// If D is non-empty, starts a forward iteration over D at the first key 
// (as defined by the order operator KEY_CMP()), then returns the first
// value. If D is empty, returns VAL_UNDEF. 
VAL_TYPE beginForward(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling beginForward() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    D->current = D->root;

    if (size(D) != 0) {
        Node N = treeMin(D->current);
        D->current = N;
        return N->val;
    }

    return VAL_UNDEF;
}

// beginReverse()
// If D is non-empty, starts a reverse iteration over D at the last key 
// (as defined by the order operator KEY_CMP()), then returns the last
// value. If D is empty, returns VAL_UNDEF.
VAL_TYPE beginReverse(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling beginReverse() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    D->current = D->root;

    if (size(D) != 0) {
        Node N = treeMax(D->current);
        D->current = N;
        return N->val;
    }

    return VAL_UNDEF;
}

// currentKey()
// If an iteration (forward or reverse) over D has started, returns the 
// the current key. If no iteration is underway, returns KEY_UNDEF.
KEY_TYPE currentKey(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling currentKey() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    if (D->current != NULL) {
        return D->current->key;
    }
    
    return KEY_UNDEF;
}

// currentVal()
// If an iteration (forward or reverse) over D has started, returns the 
// value corresponding to the current key. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE currentVal(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling currentVal() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    if (D->current != NULL) {
        return D->current->val;
    }

    return VAL_UNDEF;
}

// next()
// If an iteration (forward or reverse) over D has started, and has not
// reached the last pair, moves to the next key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the last pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF.
VAL_TYPE next(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling next() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    if (D->current != NULL && D->current->key != KEY_UNDEF) {
        Node N = treeSuccessor(D, D->current);
        D->current = N;
        return N->val;
    }
    
    return VAL_UNDEF;
}

// prev()
// If an iteration (forward or reverse) over D has started, and has not
// reached the first pair, moves to the previous key in D (as defined by the 
// order operator KEY_CMP()), and returns the value corresponding to the 
// new key. If an iteration has started, and has reached the first pair,
// ends the iteration and returns VAL_UNDEF. If no iteration is underway, 
// returns VAL_UNDEF. 
VAL_TYPE prev(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling prev() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    if (D->current != NULL && D->current->key != KEY_UNDEF) {
        Node N = treePredecessor(D, D->current);
        D->current = N;
        return N->val;
    }

    return VAL_UNDEF;
}


// Other operations -----------------------------------------------------------

// inOrderPrintTree()
// Helper function for printDictionary(). Prints key-value pairs in-order from 
// a tree with a root of Node N.
void inOrderPrintTree(FILE *out, Node N) {
    if (N != NULL) {
        inOrderPrintTree(out, N->left);
        fprintf(out, "key: "KEY_FORMAT" value: "VAL_FORMAT"\n", N->key, N->val);
        inOrderPrintTree(out, N->right);
    }
}

// printDictionary()
// Prints a text representation of D to the file pointed to by out. Each key-
// value pair is printed on a single line, with the two items separated by a
// single space. The pairs are printed in the order defined by the operator
// KEY_CMP().
void printDictionary(FILE* out, Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling printDictionary() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    inOrderPrintTree(out, D->root);
    
    return;
}