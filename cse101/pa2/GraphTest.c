// ---------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA2
// GraphTest.c
// Test client for Graph ADT
// ---------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"

int main(void){
    Graph G = newGraph(6);

    addEdge(G, 1, 2);
    addEdge(G, 1, 2);
    addEdge(G, 1, 3);
    addEdge(G, 2, 1);
    addEdge(G, 2, 4);
    addEdge(G, 2, 5);
    addEdge(G, 2, 6);
    addEdge(G, 3, 1);
    addEdge(G, 3, 4);
    addEdge(G, 4, 2);
    addEdge(G, 4, 3);
    addEdge(G, 4, 5);
    addEdge(G, 5, 2);
    addEdge(G, 5, 4);
    addEdge(G, 5, 6);
    addEdge(G, 6, 2);
    addEdge(G, 6, 5);
    printGraph(stdout, G);

    int s = 4;
    int u = 4;

    BFS(G, s);
    
    List path = newList();
    getPath(path, G, u);

    printf("\nThe distance from %d to %d is %d\nA shortest %d-%d path is: ", s, u, getDist(G, u), s, u);

    printList(stdout, path);

    freeGraph(&G);

    return 0;
}