// ---------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA3
// FindComponents.c
// Top-level client and main program for PA3. 
// ---------------------------------------------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include "Graph.h"

int main(int argc, char **argv) {
    // Variables for input and output files.
    FILE *infile = NULL, *outfile = NULL;

    // Set input and output files from command-line arguments.
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        return 1;
    }
    
    infile = fopen(argv[1], "r");
    if (infile == NULL) {
        fprintf(stderr, "Unable to open file %s for reading.\n", argv[1]);
        return 1;
    }

    outfile = fopen(argv[2], "w");
    if (outfile == NULL) {
        fprintf(stderr, "Unable to open file %s for writing.\n", argv[2]);
        return 1;
    }

    int order;
    fscanf(infile, "%d\n", &order);

    Graph G = newGraph(order);

    int u, v;
    while (fscanf(infile, "%d %d\n", &u, &v) != EOF) {
        if (u == 0 && v == 0) {
            break;
        }

        addArc(G, u, v);
    }

    fprintf(outfile, "Adjacency list representation of G:\n");
    printGraph(outfile, G);

    List S = newList();
    for (int i = 1; i <= getOrder(G); i += 1) {
        append(S, i);
    }

    DFS(G, S);

    Graph T = transpose(G);

    DFS(T, S);

    int numComps = 0;
    for (int i = 1; i <= getOrder(T); i += 1) {
        if (getParent(T, i) == NIL) {
            numComps += 1;
        }
    }

    fprintf(outfile, "\nG contains %d strongly connected components:", numComps);

    // i: Component number. j: Number of vertices for each component. 
    int i = 0, j = 0;
    for (int x = 0; x < numComps; x += 1) {
        // Move cursor back until parent[index] == 0.
        moveBack(S);
        while (getParent(T, get(S)) != 0) {
            movePrev(S);
        }

        // Print out all vertices in component.
        i += 1;
        fprintf(outfile, "\nComponent %d:", i);
        while (index(S) >= 0) {
            j += 1;
            fprintf(outfile, " %d", get(S));
            moveNext(S);
        }

        // Delete vertices in component.
        for (int k = 0; k < j; k += 1) {
            deleteBack(S);
        }

        j = 0;
    }

    fprintf(outfile, "\n");

    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
    fclose(infile);
    fclose(outfile);
    
    return 0;
}