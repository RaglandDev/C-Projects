#include "universe.h"

#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "tsn:i:o:h"
#define DELAY   50000

int main(int argc, char **argv) {
    // Defaults
    int opt = 0;
    int generations = 100;

    FILE *input = stdin;
    FILE *output = stdout;

    bool silent = false;
    bool donut = false;
    bool help = false;
    bool run = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't': donut = true; break;
        case 's': silent = true; break;
        case 'n': generations = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'i':
            input = fopen(optarg, "r");
            if (input == 0)
            {
                return -1;
            }
            run = true;
            break;
        case 'o': output = fopen(optarg, "w");
        if (output == 0)
        {
            return -1;
        } 
        break;
        case 'h': help = true; break;
        }
    }

    if (help) {
        printf("SYNOPSIS\n\tConway's Game of Life\n\nUSAGE\n\t./life "
               "tsn:i:o:h\n\nOPTIONS\n\t-t\t\tCreate your universe as a toroidal\n\t-s\t\tSilent - "
               "do not use animate the evolution\n\t-n {number}\tNumber of generations [default: "
               "100]\n\t-i {file}\tInput file [default: stdin]\n\t-o {file}\tOutput file [default: "
               "stdout]\n");
    }
    if (input) {

        if (run) {
            int r = 0;
            int c = 0;

            fscanf(input, "%d %d", &r, &c);

            // Creation & population of the universe
            Universe *a = uv_create(r, c, donut);
            uv_populate(a, input);

            Universe *b = uv_create(r, c, donut);
            uv_populate(b, input);

            if (!silent) {
                initscr();
                curs_set(FALSE);
            }
            for (int gen = 0; gen < generations; gen++) {
                if (!silent) {
                    clear();
                }

                // Loop over every cell and census it
                for (uint32_t i = 0; i < uv_rows(a); i++) {
                    for (uint32_t j = 0; j < uv_rows(a); j++) {
                        int live_neighbors = uv_census(a, i, j);

                        // Live cell with 2 live neighbors
                        if (uv_get_cell(a, i, j) && ((live_neighbors == 2))) {
                            uv_live_cell(b, i, j);
                            if (!silent) {
                                mvprintw(i, j, "o");
                            }
                        }
                        // Any cell with 3 live neighbors
                        else if (live_neighbors == 3) {
                            uv_live_cell(b, i, j);
                            if (!silent) {
                                mvprintw(i, j, "o");
                            }
                        }
                        // Cell dies
                        else {
                            uv_dead_cell(b, i, j);
                        }
                    }
                }

                if (!silent) {
                    refresh();
                    usleep(DELAY);
                }
                // Swap universes
                Universe *temp = a;
                a = b;
                b = temp;
            }

            endwin();
            uv_print(a, output);
            fflush(output);
            uv_delete(a);
            uv_delete(b);

            fclose(input);
            fclose(output);
        }
    } else {
        return false;
    }
    return 0;
}
