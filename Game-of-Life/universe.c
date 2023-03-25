#include "universe.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

// Creates and returns a Universe of specified rows, columns, and donut-ness
// Returns NULL if memory allocation fails during creation
//
// rows: The number of rows in the Universe grid
// cols: The number of columns in the Universe grid
// toroidal: Whether or not the Universe is donut-shaped
Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *u = (Universe *) malloc(sizeof(Universe));
    if (u != NULL) {
        u->rows = rows;
        u->cols = cols;
        u->toroidal = toroidal;
        u->grid = (bool **) calloc(rows, sizeof(bool *));

        for (uint32_t r = 0; r < rows; r++) {
            u->grid[r] = (bool *) calloc(cols, sizeof(bool));
        }

        if (!u->grid) {
            free(u);
            u = NULL;
        }
    }
    return u;
}

// Frees all memory used by Universe struct
// Returns void
//
// u: Pointer to Universe struct
void uv_delete(Universe *u) {
    for (uint32_t r = 0; r < u->rows; r++) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
}

// Returns number of rows in Universe
//
// u: Pointer to Universe struct
uint32_t uv_rows(Universe *u) {
    return u->rows;
}

// Returns number of columns in Universe
//
// u: Pointer to Universe struct
uint32_t uv_cols(Universe *u) {
    return u->cols;
}

// Marks a cell within Universe as live (true)
// Returns void
//
// u: Pointer to Universe struct
// r: Row index
// c: Column index
void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if ((r > uv_rows(u) || (c > uv_cols(u)))) {
        printf("WARNING: cannot mark cell as live outside of grid");
    } else {
        u->grid[r][c] = true;
    }
}

// Marks a cell within Universe as dead (false)
// Returns void
//
// u: Pointer to Universe struct
// r: Row index
// c: Column index
void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if ((r > uv_rows(u)) || (c > uv_cols(u))) {
        printf("WARNING: cannot mark cell as dead outside of grid");
    } else {
        u->grid[r][c] = false;
    }
}

// Retrieves the value of a cell within Universe
// Returns true if cell is alive, false if cell is dead or out of range
//
// u: Pointer to Universe struct
// r: Row index
// c: Column index
bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if ((r > uv_rows(u)) || (c > uv_cols(u))) {
        printf("WARNING: cannot get cell outside of grid");
        return false;
    } else if (u->grid[r][c] == true) {
        return true;
    } else {
        return false;
    }
}

// Sets specified Universe cells as alive
//
// u: Pointer to Universe struct
// infile: Pointer to file stream
bool uv_populate(Universe *u, FILE *infile) {
    uint32_t l, r;
    while (fscanf(infile, "%d %d", &l, &r) == 2) {
        if ((l > u->rows) || (r > u->cols)) {
            printf("WARING: cannnot populate cell outside of grid");
            return false;
        } else {
            u->grid[l][r] = true;
        }
    }
    return true;
}

// Returns number of live neighbors
//
// u: Pointer to Universe struct
// r: Row index
// c: Column index
// pnr = Possible neighbor row
// pnc = Possible neighbor column
uint32_t uv_census(Universe *u, uint32_t rr, uint32_t cc) {
    int live_neighbors_c = 0;
    int64_t r = rr;
    int64_t c = cc;

    // 3x3 centered on cell
    for (int64_t i = (r - 1); i <= (r + 1); i++) {
        for (int64_t j = (c - 1); j <= (c + 1); j++) {
            // Outside grid
            if ((i < 0) || (i >= uv_rows(u)) || (j < 0) || (j >= uv_cols(u))) {
                if (u->toroidal) {
                    int64_t ii = i;
                    int64_t jj = j;
                    do {

                        // There is probably a way cleverer way to do this but I don't care
                        if ((ii < 0)) {
                            ii = uv_rows(u) - 1;
                        }
                        if ((jj < 0)) {
                            jj = uv_cols(u) - 1;
                        }
                        if (ii >= uv_rows(u)) {
                            ii = i % uv_rows(u);
                        }
                        if (jj >= uv_cols(u)) {
                            jj = j % uv_cols(u);
                        }
                    } while ((ii < 0) || (ii >= uv_rows(u)) || (jj < 0) || (jj >= uv_cols(u)));

                    if (uv_get_cell(u, ii, jj)) {
                        live_neighbors_c++;
                    }
                }
                continue;
                // Same cell
            } else if ((i == r) && (j == c)) {
                continue;
            }
            // Live neighbor
            else if (uv_get_cell(u, i, j)) {
                live_neighbors_c++;
            }
        }
    }
    return live_neighbors_c;
}

// Prints Universe to outfile
//
// u: Pointer to Universe struct
// outfile: Pointer to output file stream
void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t r = 0; r < uv_rows(u); r++) {
        for (uint32_t c = 0; c < uv_cols(u); c++) {
            if (u->grid[r][c] == true) {
                fputs("o", outfile);
            } else {
                fputs(".", outfile);
            }
        }
        fputs("\n", outfile);
    }
}
