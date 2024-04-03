//-----------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE 101 PA7
// Dictionary.c 
// Implementation file for Dictionary ADT
//-----------------------------------------------------------------------------

#include "Dictionary.h"

#define BLACK 0
#define RED 1

// Structs --------------------------------------------------------------------

typedef struct NodeObj {
    int color;
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

    N->color = BLACK;
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

    // Create nil Node.
    Node nil = newNode(KEY_UNDEF, VAL_UNDEF);

    D->root = nil;
    D->size = 0;
    D->current = nil;
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
Node lookupNode(Dictionary D, Node N, KEY_TYPE k) {
    if (N == D->nil || KEY_CMP(k, N->key) == 0) {
        return N;

    } else if (KEY_CMP(k, N->key) < 0) {
        return lookupNode(D, N->left, k);

    } else {
        return lookupNode(D, N->right, k);
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

    if (size(D) != 0) {
        Node N = lookupNode(D, D->root, k);

        if (N != NULL) {
            return N->val;
        }
    }

    return VAL_UNDEF;
}


// Manipulation procedures ----------------------------------------------------

// treeMin()
// Returns the Node with the smallest key in a tree. The iteration starts with
// Node N.
// Pre: N != NULL.
Node treeMin(Dictionary D, Node N) {
    if (N == NULL) {
        fprintf(stderr, "Dictionary Error: calling treeMin() with a NULL Node.\n");
        exit(EXIT_FAILURE);
    }

    while (N != NULL && N->left != D->nil) {
        N = N->left;
    }

    return N;
}

// treeMax()
// Returns the node with the largest key in a tree with Node N as the root of
// the tree.
// Pre: N != NULL.
Node treeMax(Dictionary D, Node N) {
    if (N == NULL) {
        fprintf(stderr, "Dictionary Error: calling treeMax() with a NULL Node.\n");
        exit(EXIT_FAILURE);
    }

    while (N != NULL && N->right != D->nil) {
        N = N->right;
    }

    return N;
}

// treeSuccessor()
// If there is a successor node of Node N, returns the successor node.
// Else, returns the predecessor node of N.
Node treeSuccessor(Dictionary D, Node N) {
    if (N->right != D->nil) {
        return treeMin(D, N->right);
    }

    Node P = N->parent;
    while (N == P->right) {
        N = P;
        P = P->parent;
    }

    return P;
}

// treePredecessor()
// If there is a predecessor node of Node N, returns the predecessor
// node. Else, returns the successor node of N.
Node treePredecessor(Dictionary D, Node N) {
    if (N->left != D->nil) {
        return treeMax(D, N->left);
    }

    Node P = N->parent;
    while (N == P->left) {
        N = P;
        P = P->parent;
    }

    return P;
}

// leftRotate()
void leftRotate(Dictionary D, Node X) {
    Node Y = X->right;

    // Turn Y's left subtree into X's right subtree.
    X->right = Y->left;
    if (Y->left != D->nil) { 
        Y->left->parent = X;
    }

    // Link Y's parent to X.
    Y->parent = X->parent;
    if (X->parent == D->nil) {
        D->root = Y;
    
    } else if (X != D->nil && X->parent->left != D->nil && KEY_CMP(X->key, X->parent->left->key) == 0) {
        X->parent->left = Y;

    } else {
        X->parent->right = Y;
    }

    // Put X on Y's left.
    Y->left = X;
    X->parent = Y;
}

// rightRotate()
void rightRotate(Dictionary D, Node X) {
    Node Y = X->left;

    // Turn Y's right subtree into X's left subtree.
    X->left = Y->right;
    if (Y->right != D->nil) { 
        Y->right->parent = X;
    }

    // Link Y's parent to X.
    Y->parent = X->parent;
    if (X->parent == D->nil) {
        D->root = Y;
    
    } else if (X != D->nil && X->parent->right != D->nil && KEY_CMP(X->key, X->parent->right->key) == 0) {
        X->parent->right = Y;

    } else {
        X->parent->left = Y;
    }

    // Put X on Y's right.
    Y->right = X;
    X->parent = Y;
}

// insertFixUp()
// Helper function for insert(). Adjusts all the Nodes' colors after modifying
// the tree with insert(). Rotate nodes if necessary.
void insertFixUp(Dictionary D, Node Z) {
    while (Z->parent->color == RED) {
        if (Z->parent->parent->left != D->nil && KEY_CMP(Z->parent->key, Z->parent->parent->left->key) == 0) {
            Node Y = Z->parent->parent->right;

            if (Y != D->nil && Y->color == RED) { // Case 1
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;

            } else {
                if (Z->parent->right != D->nil && KEY_CMP(Z->key, Z->parent->right->key) == 0) { // Case 2
                    Z = Z->parent;
                    leftRotate(D, Z);
                }

                // Case 3
                Z->parent->color = BLACK;
                Z->parent->parent->color = RED;
                rightRotate(D, Z->parent->parent);
            } 

        } else {
            Node Y = Z->parent->parent->left;

            if (Y != D->nil && Y->color == RED) { // Case 4
                Z->parent->color = BLACK;
                Y->color = BLACK;
                Z->parent->parent->color = RED;
                Z = Z->parent->parent;
            
            } else {
                if (Z->parent->left != D->nil && KEY_CMP(Z->key, Z->parent->left->key) == 0) { // Case 5
                    Z = Z->parent;
                    rightRotate(D, Z);
                }

                // Case 6
                Z->parent->color = BLACK;
                Z->parent->parent->color = RED;
                leftRotate(D, Z->parent->parent);
            }
        }
    }

    D->root->color = BLACK;
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

    Node Y = D->nil;
    Node X = D->root;
    Node Z = newNode(k, v);

    while (X != D->nil) {
        Y = X;
        
        if (KEY_CMP(Z->key, X->key) < 0) {
            X = X->left;
        
        } else {
            X = X->right;
        }
    }

    Z->parent = Y;

    if (Y == D->nil) {
        D->root = Z;

    } else if (Z != NULL && Y != NULL && KEY_CMP(Z->key, Y->key) < 0) {
        Y->left = Z;

    } else {
        Y->right = Z;
    }

    Z->left = D->nil;
    Z->right = D->nil;
    Z->color = RED;
    insertFixUp(D, Z);
    
    D->size += 1;
    return;
}

// transplant()
// Helper function for delete(). Replaces Node U with Node V in the BST.
// Adjusts the other nodes accordingly.
void transplant(Dictionary D, Node U, Node V) {
    if (U->parent == D->nil) {
        D->root = V;

    } else if (U == U->parent->left) {
        U->parent->left = V;

    } else {
        U->parent->right = V;
    }

    V->parent = U->parent;
}

// deleteFixUp()
// Helper function for delete(). Adjusts all the Nodes' colors after modifying
// the tree with delete(). Rotate nodes if necessary.
void deleteFixUp(Dictionary D, Node X) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling delete() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    while (X != D->root && X->color == BLACK) {
        if (X == X->parent->left) {
            Node W = X->parent->right;

            if (W->color == RED) { // Case 1
                W->color = BLACK;
                X->parent->color = RED;
                leftRotate(D, X->parent);
                W = X->parent->right;
            }

            if (W->left->color == BLACK && W->right->color == BLACK) { // Case 2
                W->color = RED;
                X = X->parent;

            } else {
                if (W->right->color == BLACK) { // Case 3
                    W->left->color = BLACK;
                    W->color = RED;
                    rightRotate(D, W);
                    W = X->parent->right;
                }

                // Case 4
                W->color = X->parent->color;
                X->parent->color = BLACK;
                W->right->color = BLACK;
                leftRotate(D, X->parent);
                X = D->root;
            }

        } else {
            Node W = X->parent->left;

            if (W->color == RED) { // Case 5
                W->color = BLACK;
                X->parent->color = RED;
                rightRotate(D, X->parent);
                W = X->parent->left;
            }

            if (W->right->color == BLACK && W->left->color == BLACK) { // Case 6
                W->color = RED;
                X = X->parent;

            } else {
                if (W->left->color == BLACK) { // Case 7
                    W->right->color = BLACK;
                    W->color = RED;
                    leftRotate(D, W);
                    W = X->parent->left;
                }

                // Case 8
                W->color = X->parent->color;
                X->parent->color = BLACK;
                W->left->color = BLACK;
                rightRotate(D, X->parent);
                X = D->root;
            }
        }
    }

    X->color = BLACK;
}

// delete()
// Remove the pair whose key is k from Dictionary D.
// Pre: lookup(D,k)!=VAL_UNDEF (i.e. D contains a pair whose key is k.)
void delete(Dictionary D, KEY_TYPE k) {
    Node Z = lookupNode(D, D->root, k);
    Node Y = Z;
    Node X = D->nil;
    int yOgColor = Y->color;

    if (Z == NULL) {
        fprintf(stderr, "Dictionary Error: calling delete() on non-existant key.\n");
        exit(EXIT_FAILURE);
    }

    if (Z->left == D->nil) {
        X = Z->right;
        transplant(D, Z, Z->right);

    } else if (Z->right == D->nil) {
        X = Z->left;
        transplant(D, Z, Z->left);

    } else {
        Y = treeMin(D, Z->right);
        yOgColor = Y->color;
        X = Y->right;

        if (Y->parent == Z) {
            X->parent = Y;

        } else {
            transplant(D, Y, Y->right);
            Y->right = Z->right;
            Y->right->parent = Y;
        }

        transplant(D, Z, Y);
        Y->left = Z->left;
        Y->left->parent = Y;
        Y->color = Z->color;
    }

    if (yOgColor == BLACK) {
        deleteFixUp(D, X);
    }

    // Set D->current to the NIL node if is equal to Z. Then, delete Z.
    if (D->current == Z) {
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
        Node N = treeMin(D, D->current);
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
        Node N = treeMax(D, D->current);
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

    Node N = D->nil;
    if ((N = treeSuccessor(D, D->current)) != D->nil) {
        D->current = N;
        return N->val;
    }
    
    D->current = N;
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

    Node N = D->nil;
    if ((N = treePredecessor(D, D->current)) != D->nil) {
        D->current = N;
        return N->val;
    }

    D->current = N;
    return VAL_UNDEF;
}


// Other operations -----------------------------------------------------------

// preOrderPrintTree()
// Helper function for printDictionary(). Prints key-value pairs pre-order from 
// a tree with a root of Node N.
void preOrderPrintTree(FILE *out, Node N) {
    if (N != NULL) {
        fprintf(out, "key: "KEY_FORMAT"\n", N->key);
        preOrderPrintTree(out, N->left);
        preOrderPrintTree(out, N->right);
    }
}

// inOrderPrintTree()
// Helper function for printDictionary(). Prints key-value pairs in-order from 
// a tree with a root of Node N.
void inOrderPrintTree(FILE *out, Node N) {
    if (N != NULL) {
        inOrderPrintTree(out, N->left);
        fprintf(out, "key: "KEY_FORMAT"\n", N->key);
        inOrderPrintTree(out, N->right);
    }
}

// postOrderPrintTree()
// Helper function for printDictionary(). Prints key-value pairs post-order from 
// a tree with a root of Node N.
void postOrderPrintTree(FILE *out, Node N) {
    if (N != NULL) {
        postOrderPrintTree(out, N->left);
        postOrderPrintTree(out, N->right);
        fprintf(out, "key: "KEY_FORMAT"\n", N->key);
    }
}

// printDictionary()
// Prints a text representation of D to the file pointed to by out. The order
// of each key printed is determined by the string ord. The string ord takes 3 
// strings, "pre," "in," and "post," for pre-order, in-order, and post-order,
// respectively. If ord does not match one of those 3 strings, nothing is
// printed. Each key is printed on a single line.
void printDictionary(FILE* out, Dictionary D, const char* ord) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling printDictionary() with a NULL Dictionary.\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(ord, "pre") == 0) {
        preOrderPrintTree(out, D->root);
    }

    if (strcmp(ord, "in") == 0) {
        inOrderPrintTree(out, D->root);
    }

    if (strcmp(ord, "post") == 0) {
        postOrderPrintTree(out, D->root);
    }

    return;
}