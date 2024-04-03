//-----------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA3
// Graph.h
// Header file for Graph ADT
//-----------------------------------------------------------------------------

#ifndef GRAPH_H_INCLUDE_ 
#define GRAPH_H_INCLUDE_

#include "List.h"

#define NIL 0
#define UNDEF -1
#define INF -2
#define WHITE 0
#define GREY 1
#define BLACK -1


// Exported type --------------------------------------------------------------

typedef struct GraphObj* Graph;


// Constructors-Destructors ---------------------------------------------------

// Create and return a reference to a new Graph object with n verticies and
// no edges.
Graph newGraph(int n);

// Free heap memory associated with *pG and sets *pG to NULL.
void freeGraph(Graph *pG);


// Access functions -----------------------------------------------------------

// Return the order of G.
int getOrder(Graph G);

// Return the size of G.
int getSize(Graph G);

// Return the sorce vertex most recently used in function BFS() or NIL if BFS()
// has not yet been called.
int getSource(Graph G);

// Return the parent of vertex u or NIL if BFS() or DFS()
// has not yet been called.
int getParent(Graph G, int u);

// Return the distance from the most recent BFS source to vertex u or INF if
// BFS() has not yet been called.
int getDist(Graph G, int u);

// Append to the List L the verticies of a shortest path in G from source to u
// or append to L the value NIL if no such path exists.
void getPath(List L, Graph G, int u);

// Return the discover time of vertex u in G. If DFS() has not been called,
// return UNDEF.
// Pre: 1 <= u <= getOrder(G).
int getDiscover(Graph G, int u);

// Return the finish time of vertex u in G. If DFS() has not been called,
// return UNDEF.
// Pre: 1 <= u <= getOrder(G).
int getFinish(Graph G, int u);


// Manipulation procedures ---------------------------------------------------

// Delete all edges of G, restoring it to its original null graph state.
void makeNull(Graph G);

// Insert a new edge in G joining u to v.
// Pre: u and v are in the range of [1, getOrder(G)]
void addEdge(Graph G, int u, int v);

// Insert a new directed edge in G from u to v. 
// Pre: u and v are in the range of [1, getOrder(G)]
void addArc(Graph G, int u, int v);

// Run the BFS algorithm on the Graph G with source s, setting the color,
// distance, parent, and source fields of G accordingly. 
void BFS(Graph G, int s);

// Perform the depth first serach algorithm on G with S defining the order in
// which vertices to be processed in the main loop of DFS and storing the
// vertices by decreasing  finish times. 
// Pre: length(S) == getOrder(G).
void DFS(Graph G, List S);


// Other operations ----------------------------------------------------------

// Return a reference to a new graph object representing the transpose of G.
Graph transpose(Graph G);

// Return a reference to a new graph that is the copy of G.
Graph copyGraph(Graph G);

// Print the adhacency list representation of G to the file pointed to by out.
void printGraph(FILE* out, Graph G);

#endif