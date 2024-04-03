#include "universe.h"

#include <stdlib.h>

struct Universe {
    int rows;
    int cols;
    bool **grid;
    bool toroidal;
};

//
// Code guided by Sahiti's 1/28/2021 section.
//
Universe *uv_create(int rows, int cols, bool toroidal) {
    Universe *u = (Universe *) calloc(1, sizeof(Universe));
    u->rows = rows;
    u->cols = cols;
    u->toroidal = toroidal;
    u->grid = (bool **) calloc(rows, sizeof(bool *));
    for (int i = 0; i < rows; i += 1) {
        u->grid[i] = (bool *) calloc(cols, sizeof(bool));
    }

    return u;
}

//
// Code from Sahiti's section on 1/28/2021.
// Frees allocated memory in a Universe u.
//
void uv_delete(Universe *u) {
    for (int r = 0; r < uv_rows(u); r += 1) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
    return;
}

//
// Returns the number of rows in a Universe u.
//
int uv_rows(Universe *u) {
    return u->rows;
}

//
// Return the number of columns in a Universe u.
//
int uv_cols(Universe *u) {
    return u->cols;
}

//
// Sets a cell as alive.
//
// u: A Universe u.
// r: A cell in row r of **grid.
// c: A cell in column c of **grid.
//
void uv_live_cell(Universe *u, int r, int c) {
    u->grid[r][c] = true;
    return;
}

//
// Sets a cell as dead.
//
// u: A Universe u.
// r: A cell in row r of **grid.
// c: A cell in column c of **grid.
//
void uv_dead_cell(Universe *u, int r, int c) {
    u->grid[r][c] = false;
    return;
}

//
// Returns the state of a cell.
//
// u: A Universe u.
// r: A cell in row r of **grid.
// c: A cell in column c of **grid.
//
bool uv_get_cell(Universe *u, int r, int c) {
    return u->grid[r][c];
}

//
// Populates a universe with instructions from a file. Return false if a cell is found to be out of the universe's bounds. Return true if the universe is successfully populated.
//
// u: A Universe u.
// infile: A FILE that specifies the number of rows and columns as well as the positions of live cells.
//
bool uv_populate(Universe *u, FILE *infile) {
    // Reads the rest of the infile.
    int r, c;
    while (fscanf(infile, "%d %d\n", &r, &c) != EOF) {
        if (r < uv_rows(u) && r >= 0 && c < uv_cols(u) && c >= 0) {
            uv_live_cell(u, r, c); // Sets the cell at r, c to true
        } else {
            return false;
        }
    }
    return true;
}

//
// Returns the position of a row or column in a toroidal universe.
//
// pos: The index of the row or column in question.
// n: The number of rows or columns in the universe.
//
int wrap(int pos, int n) {
    return (pos + n) % n;
}

//
// Returns the number of adjacent cells for a specified cell.
//
// u: A Universe u.
// r: The cell's row index.
// c: The cell's column index.
//
int uv_census(Universe *u, int r, int c) {
    int count = 0;

    // Inspired by Eugene's section on 1/29/2021
    for (int row = r - 1; row <= r + 1; row += 1) {
        for (int col = c - 1; col <= c + 1; col += 1) {
            if (row == r && col == c) {
                continue;
            }
            if (u->toroidal == true) {
                if (uv_get_cell(u, wrap(row, uv_rows(u)), wrap(col, uv_cols(u))) == true) {
                    count += 1;
                }
            } else {
                if (row >= 0 && row < uv_rows(u) && col >= 0 && col < uv_cols(u)
                    && uv_get_cell(u, row, col) == true) {
                    count += 1;
                }
            }
        }
    }
    return count;
}

//
// Prints out a universe to a file. A live cell is indicated with the character 'o'. A dead cell is indicated with '.'. The printed universe will always be flat.
//
// u: A Universe u.
// outfile: The file to write the universe in.
//
void uv_print(Universe *u, FILE *outfile) {
    for (int r = 0; r < uv_rows(u); r += 1) {
        for (int c = 0; c < uv_cols(u); c += 1) {
            if (uv_get_cell(u, r, c) == true) {
                fprintf(outfile, "o"); // Print 'o' if alive.
            } else {
                fprintf(outfile, "."); // Print '.' if dead.
            }
        }
        fprintf(outfile, "\n");
    }
}
