#include "universe.h"

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:"
#define DELAY   50000

int main(int argc, char **argv) {
    // Variables for command-line options.
    int opt;
    bool tor = false, sil = false;
    int gens = 100;
    FILE *input = stdin, *output = stdout;

    // Loops through all inputted command-line options.
    // n, i, and o cases were inspired by Sahiti's section on 1/28/2021.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': tor = true; break;
        case 's': sil = true; break;
        case 'n':
            gens = atoi(optarg);
            if (gens == 0) {
                fprintf(stderr, "-n only accepts integers.\n");
                return 0;
            } else if (gens < 0) {
                fprintf(stderr, "Invalid number of generations.\n");
                return 0;
            }
            break;
        case 'i':
            input = fopen(optarg, "r");
            if (input == NULL) {
                fprintf(stderr, "File provided for input is NULL.\n");
                return 0;
            }
            break;
        case 'o': output = fopen(optarg, "w"); break;
        default:
            fprintf(stderr, "Usage: %s -[tsn:i:o:]\n", argv[0]);
            return 0;
            break;
        }
    }

    int nrows = 0, ncols = 0;
    fscanf(input, "%d %d\n", &nrows, &ncols);

    // Error handling for number of rows/columns.
    if (nrows < 0 || ncols < 0) {
        fprintf(stderr, "Invalid number of rows or columns.\n");
        return 0;
    }

    Universe *a = uv_create(nrows, ncols, tor);
    Universe *b = uv_create(nrows, ncols, tor);

    if (uv_populate(a, input) == false) {
        fprintf(stderr, "Universe failed to populate.\n");
        return 0;
    }

    if (sil == false) {
        initscr(); // Initialize the screen.
        noecho();
        curs_set(FALSE); // Hide the cursor.
    }

    for (int i = 0; i < gens; i += 1) {
        if (sil == false) {
            for (int r = 0; r < nrows; r += 1) {
                for (int c = 0; c < ncols; c += 1) {
                    if (uv_get_cell(a, r, c) == true) {
                        mvprintw(r, c, "o"); // Displays "o".
                    }
                }
            }
            refresh(); // Refresh the window.
            usleep(DELAY); // Sleep for 50000 microseconds.
            clear(); // Clear the window.
        }

        // Code from Sahiti's section on 1/28/2021.
        // Revives cells with 3 neighboring live cells.
        // Kills cells without 2 or 3 neighboring live cells.
        for (int r = 0; r < nrows; r += 1) {
            for (int c = 0; c < ncols; c += 1) {
                int count = uv_census(a, r, c);
                if (uv_get_cell(a, r, c) == true && (count == 2 || count == 3)) {
                    uv_live_cell(b, r, c);
                }
                if (uv_get_cell(a, r, c) == false && count == 3) {
                    uv_live_cell(b, r, c);
                }
                if (count < 2 || count > 3) {
                    uv_dead_cell(b, r, c);
                }
            }
        }

        // Swap Universe A and B.
        Universe *temp = a;
        a = b;
        b = temp;

        // Clear Universe B of any live cells.
        for (int r = 0; r < nrows; r += 1) {
            for (int c = 0; c < ncols; c += 1) {
                uv_dead_cell(b, r, c);
            }
        }
    }

    if (sil == false) {
        endwin(); // Close the screen.
    }

    uv_print(a, output); // Prints Universe A into output file.

    // Closes input/output files.
    fclose(input);
    fclose(output);

    // Frees allocated memory in Universes.
    uv_delete(a);
    uv_delete(b);

    return 0;
}
