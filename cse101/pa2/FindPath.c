// ---------------------------------------------------------------------------
// Tu-An Nguyen, tunhnguy
// 2021 Spring CSE101 PA2
// Graph.c
// Implementation file for Graph ADT
// ---------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
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

        addEdge(G, u, v);
    }

    printGraph(outfile, G);

    List L = newList();

    int s, d;
    while (fscanf(infile, "%d %d\n", &s, &d) != EOF) {
        if (s == 0 && d == 0) {
            break;
        }

        BFS(G, s);
        getPath(L, G, d);

        if (getDist(G, d) != INF) {
            fprintf(outfile, "\nThe distance from %d to %d is %d\nA shortest %d-%d path is: ", s, d, getDist(G, d), s, d);

            printList(outfile, L);
            fprintf(outfile, "\n");

            clear(L);

        } else {
            fprintf(outfile, "\nThe distance from %d to %d is infinity\nNo %d-%d path exists", s, d, s, d);
            fprintf(outfile, "\n");

            clear(L);
        }
    }

    fclose(infile);
    fclose(outfile);
    freeList(&L);
    freeGraph(&G);
    
    return 0;
}