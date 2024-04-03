// ---------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA2
// Graph.c
// Implementation file for Graph ADT
// ---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"


// Structs -------------------------------------------------------------------

typedef struct GraphObj {
    List *neighbors;
    int *color;
    int *parent;
    int *distance;
    int order;
    int size;
    int source;
} GraphObj;


// Constructors-Destructors ---------------------------------------------------

// Create and return a reference to a new Graph object with n verticies and
// no edges.
Graph newGraph(int n) {
    Graph G = (Graph) malloc(sizeof(GraphObj));

    if (G) {
        G->neighbors = (List *) calloc(n + 1, sizeof(List));

        // Initialize empty lists for each vertex in neighbors if calloc()
        // for G->neighbors was successful.
        if (G->neighbors) {
            for (int i = 1; i <= n; i += 1) {
                G->neighbors[i] = newList(); 
            }
        }

        G->color = (int *) calloc(n + 1, sizeof(int));
        G->parent = (int *) calloc(n + 1, sizeof(int));
        G->distance = (int *) calloc(n + 1, sizeof(int));

        G->order = n;
        G->size = 0;
        G->source = NIL; 
    }

    return G;
}

// Free heap memory associated with *pG and sets *pG to NULL.
void freeGraph(Graph *pG) {
    // Free Lists objects in neighbors.
    for (int i = 0; i <= getOrder(*pG); i += 1) {
        freeList(&(*pG)->neighbors[i]);
    }

    free((*pG)->neighbors);
    free((*pG)->color);
    free((*pG)->parent);
    free((*pG)->distance);
 
    (*pG)->neighbors = NULL;
    (*pG)->color = NULL;
    (*pG)->parent = NULL;
    (*pG)->distance = NULL;

    free(*pG);
    *pG = NULL;

    return;
}


// Access functions -----------------------------------------------------------

// Return the order of G.
// Pre: G is not NULL.
int getOrder(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling getOrder() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    return G->order;
}

// Return the size of G.
// Pre: G is not NULL.
int getSize(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling getSize() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    return G->size;
}

// Return the sorce vertex most recently used in function BFS() or NIL if BFS()
// has not yet been called.
// Pre: G is not NULL.
int getSource(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling getSource() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    if (G->source != NIL) {
        return G->source;
    }

    return NIL;
}

// Return the parent of vertex u in the Breadth-First tree created by BFS() or
// NIL if BFS() has not yet been called.
// Pre: G is not NULL. 1 <= u <= getOrder(G).
int getParent(Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling getParent() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G))  {
        fprintf(stderr, "Graph Error: calling getParent() with a non existent vertex.\n");
        exit(EXIT_FAILURE);
    }
    
    if (getSource(G) != NIL) {
        return G->parent[u];
    }

    return NIL;
}

// Return the distance from the most recent BFS source to vertex u or INF if
// BFS() has not yet been called.
// Pre: G is not NULL. 1 <= u <= getOrder(G).
int getDist(Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling getDist() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G))  {
        fprintf(stderr, "Graph Error: calling getDist() with a non existent vertex.\n");
        exit(EXIT_FAILURE);
    }
    
    if (G->source != NIL) {
        return G->distance[u];
    }

    return INF;
}

// Append to the List L the verticies of a shortest path in G from source to u
// or append to L the value NIL if no such path exists.
// Pre: G is not NULL. getSource(G) != NIL / BFS() was ran. 1 <= u <= getOrder(G).
void getPath(List L, Graph G, int u) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling getPath() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }

    if (u < 1 || u > getOrder(G))  {
        fprintf(stderr, "Graph Error: calling getPath() with a non existent vertex.\n");
        exit(EXIT_FAILURE);
    }

    if (getSource(G) == NIL) {
        fprintf(stderr, "Graph Error: calling getPath() on a NIL source vertex.\n");
        exit(EXIT_FAILURE);
    }
    
    if (u == getSource(G)) {
        append(L, u);

    } else if (getParent(G, u) == NIL) {
        append(L, NIL);
    
    } else {
        getPath(L, G, getParent(G, u));
        append(L, u);
    }
    
    return;
}


// Manipulation procedures ---------------------------------------------------

// Delete all edges of G, restoring it to its original null graph state.
// Pre: G is not NULL or empty.
void makeNull(Graph G) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling makeNull() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    if (getOrder(G) == 0) {
        fprintf(stderr, "Graph Error: calling makeNull() on an empty Graph.\n");
        exit(EXIT_FAILURE);
    }

    G->color = (int *) calloc(getOrder(G) + 1, sizeof(int));
    G->parent = (int *) calloc(getOrder(G) + 1, sizeof(int));
    G->distance = (int *) calloc(getOrder(G) + 1, sizeof(int));

    G->size = 0;
    G->source = 0; 

    for (int i = 0; i < getOrder(G); i += 1) {
        clear(G->neighbors[i]);
    }

    G->size = 0;
    
    return;
}

// Insert a new edge in G joining u to v.
// Pre: 1 <= u, v <= getOrder(G). G is not NULL or empty. 
 void addEdge(Graph G, int u, int v) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling addEdge() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    if (getOrder(G) == 0) {
        fprintf(stderr, "Graph Error: calling addEdge() on an empty Graph.\n");
        exit(EXIT_FAILURE);
    }

    if ((u < 1 || u > getOrder(G)) || (v < 1 || v > getOrder(G))) {
        fprintf(stderr, "Graph Error: calling addEdge() with a non existent vertex.\n");
        exit(EXIT_FAILURE);
    }

    addArc(G, u, v);
    addArc(G, v, u);

    // Account for extra edge counted when adding both arcs.
    G->size -= 1;

    return;
}

// Insert a new directed edge in G from u to v. 
// Pre: 1 <= u, v <= getOrder(G). G is not NULL or empty. 
void addArc(Graph G, int u, int v) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling addArc() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    if (getOrder(G) == 0) {
        fprintf(stderr, "Graph Error: calling addArc() on an empty Graph.\n");
        exit(EXIT_FAILURE);
    }

    if ((u < 1 || u > getOrder(G)) || (v < 1 || v > getOrder(G))) {
        fprintf(stderr, "Graph Error: calling addArc() with a non existent vertex.\n");
        exit(EXIT_FAILURE);
    }

    if (length(G->neighbors[u]) == 0) { // u vertex adjacency list is empty.
        append(G->neighbors[u], v);
        G->size += 1;
        return;
    }
    
    moveFront(G->neighbors[u]);

    // Move cursor to find position to put v in.
    // Maintain convention of increasing vertex labels
    // in the adjacency list.
    while (index(G->neighbors[u]) >= 0 && get(G->neighbors[u]) <= v) {
        if (get(G->neighbors[u]) == v) { // The pair (u, v) already exists in the graph.
            return;

        } else {
            moveNext(G->neighbors[u]);
        }
    }

    if (index(G->neighbors[u]) >= 0) {
        insertBefore(G->neighbors[u], v);

    } else {
        append(G->neighbors[u], v);
    }

    G->size += 1;

    return;
}

// Run the BFS algorithm on the Graph G with source s, setting the color,
// distance, parent, and source fields of G accordingly.
// Pre: G is not NULL or empty and source s is in G. 
void BFS(Graph G, int s) {
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling BFS() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    if (getOrder(G) == 0) {
        fprintf(stderr, "Graph Error: calling BFS() on an empty Graph.\n");
        exit(EXIT_FAILURE);
    }

    if (s < 1 || s > getOrder(G)) {
        fprintf(stderr, "Graph Error: calling BFS() with a non existent source vertex.\n");
        exit(EXIT_FAILURE);
    }

    G->source = s; // Set source vertex with s in G.

    for (int i = 1; i <= getOrder(G); i += 1) {
        G->color[i] = WHITE;
        G->distance[i] = INF;
        G->parent[i] = NIL;
    }

    G->color[s] = GREY;
    G->distance[s] = 0;
    G->parent[s] = NIL;

    List Q = newList();
    append(Q, s);

    while (length(Q) != 0) {
        // Dequeue and set the value to X.
        int x = front(Q);
        deleteFront(Q);

        if (length(G->neighbors[x]) == 0) { // Vertex x's adjacency list is empty.
            G->color[x] = BLACK; // Finish x.
            continue;
        }

        moveFront(G->neighbors[x]);
        while (index(G->neighbors[x]) >= 0) {
            int y = get(G->neighbors[x]);
            
            if (G->color[y] == WHITE) { // y is undiscovered.
                G->color[y] = GREY; // Discover y.
                G->distance[y] = G->distance[x] + 1;
                G->parent[y] = x;

                append(Q, y); // Enqueue y into Q.
            }

            moveNext(G->neighbors[x]);
        }

        G->color[x] = BLACK; // Finish x.
    } 

    freeList(&Q);

    return;
}


// Other operations ----------------------------------------------------------

// Print the adhacency list representation of G to the file pointed to by out.
// Pre: G is not NULL or empty. out is not NULL.
void printGraph(FILE* out, Graph G) {
    if (out == NULL) {
        fprintf(stderr, "Graph Error: calling printGraph() on a NULL output file.\n");
        exit(EXIT_FAILURE);
    }
    
    if (G == NULL) {
        fprintf(stderr, "Graph Error: calling printGraph() on a NULL Graph reference.\n");
        exit(EXIT_FAILURE);
    }
    
    if (getOrder(G) == 0) {
        fprintf(stderr, "Graph Error: calling printGraph() on an empty Graph.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= getOrder(G); i += 1) {
        fprintf(out, "%d: ", i);
        if (length(G->neighbors[i]) != 0) {
            printList(out, G->neighbors[i]);
        }

        fprintf(out, "\n");
    }
    
    return;
}