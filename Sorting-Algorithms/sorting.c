#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Sort functions
#include "batcher.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"
#include "set.h"

#define OPTIONS "gahbsqr:n:p:H"

#define HELP         (uint8_t) 0
#define SHELL_SORT   (uint8_t) 1
#define QUICK_SORT   (uint8_t) 2
#define HEAP_SORT    (uint8_t) 3
#define BATCHER_SORT (uint8_t) 4
#define SET_SEED     (uint8_t) 5
#define ARRAY_SIZE   (uint8_t) 6
#define PRINT_ELE    (uint8_t) 7

int main(int argc, char **argv) {
    bool graph = false;

    int opt = 0;

    // Default seed, array size, printed elements
    uint32_t seed = 13371453;
    uint32_t size = 100;
    uint32_t elements = 100;
    Set flags = set_empty();

    // Set flags
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) // CITATION: Aubrey Ostrom 1/30/23
    {
        switch (opt) {
        case 'a':
            flags = set_universal();
            flags = set_remove(flags, HELP);

            break;
        case 's': flags = set_insert(flags, SHELL_SORT); break;
        case 'h': flags = set_insert(flags, HEAP_SORT); break;
        case 'q': flags = set_insert(flags, QUICK_SORT); break;
        case 'b': flags = set_insert(flags, BATCHER_SORT); break;
        case 'r':
            flags = set_insert(flags, SET_SEED);
            seed = (uint32_t) strtoul(optarg, NULL, 10);
            break;
        case 'n':
            flags = set_insert(flags, ARRAY_SIZE);
            size = (uint32_t) strtoul(optarg, NULL, 10);
            break;
        case 'p':
            flags = set_insert(flags, PRINT_ELE);
            elements = (uint32_t) strtoul(optarg, NULL, 10);
            break;
        case 'H': flags = set_insert(flags, HELP); break;
        case 'g': graph = true; break;
        }
    }

    clock_t before = clock();

    // Iteration loop helper
    uint32_t i;
    // Check flags
    if (set_member(flags, SHELL_SORT)) {
        // Allocate memory for initial array
        uint32_t *s_array = (uint32_t *) malloc(size * sizeof(uint32_t));
        // Set pseudorandom seed
        srandom(seed);
        // Fill initial array with random numbers
        for (i = 0; i < size; i++) {
            s_array[i] = (random() & 0x3FFFFFFF);
        }

        Stats shell_stats;
        reset(&shell_stats);
        shell_sort(&shell_stats, s_array, size);
        if (graph == false) {
            printf("Shell Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size,
                shell_stats.moves, shell_stats.compares);

            // Print x number of elements
            for (i = 0; i < elements; i++) {
                if ((i % 5 == 0) && (i != 0)) {
                    printf("\n");
                }
                printf("%12d", s_array[i]);
            }
            printf("\n");
        }
        if (graph == true) {
            printf("%d\t%" PRIu64, size, shell_stats.moves);
            printf("\n");
        }
        free(s_array);
    }

    if (set_member(flags, QUICK_SORT)) {
        // Allocate memory for initial array
        uint32_t *s_array = (uint32_t *) malloc(size * sizeof(uint32_t));
        // Set pseudorandom seed
        srandom(seed);
        // Fill initial array with random numbers
        for (i = 0; i < size; i++) {
            s_array[i] = (random() & 0x3FFFFFFF);
        }

        Stats quick_stats;
        reset(&quick_stats);
        quick_sort(&quick_stats, s_array, size);
        if (graph == false) {
            printf("Quick Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size,
                quick_stats.moves, quick_stats.compares);

            // Print x number of elements
            for (i = 0; i < elements; i++) {
                if ((i % 5 == 0) && (i != 0)) {
                    printf("\n");
                }
                printf("%12d", s_array[i]);
            }
            printf("\n");
        }
        if (graph == true) {
            printf("%d\t%" PRIu64, size, quick_stats.moves);
            printf("\n");
        }
        free(s_array);
    }

    if (set_member(flags, BATCHER_SORT)) {
        // Allocate memory for initial array
        uint32_t *s_array = (uint32_t *) malloc(size * sizeof(uint32_t));
        // Fill initial array with random numbers
        // Set pseudorandom seed
        srandom(seed);
        for (i = 0; i < size; i++) {
            s_array[i] = (random() & 0x3FFFFFFF);
        }

        Stats batcher_stats;
        reset(&batcher_stats);
        batcher_sort(&batcher_stats, s_array, size);
        if (graph == false) {
            printf("Batcher Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size,
                batcher_stats.moves, batcher_stats.compares);

            // Print x number of elements
            for (i = 0; i < elements; i++) {
                if ((i % 5 == 0) && (i != 0)) {
                    printf("\n");
                }
                printf("%12d", s_array[i]);
            }
            printf("\n");
        }
        if (graph == true) {
            printf("%d\t%" PRIu64, size, batcher_stats.moves);
            printf("\n");
        }
        free(s_array);
    }

    if (set_member(flags, HEAP_SORT)) {
        // Allocate memory for initial array
        uint32_t *s_array = (uint32_t *) malloc(size * sizeof(uint32_t));
        // Fill initial array with random numbers
        // Set pseudorandom seed
        srandom(seed);
        for (i = 0; i < size; i++) {
            s_array[i] = (random() & 0x3FFFFFFF);
        }

        Stats heap_stats;
        reset(&heap_stats);
        heap_sort(&heap_stats, s_array, size);
        if (graph == false) {
            printf("Heap Sort, %d elements, %" PRIu64 " moves, %" PRIu64 " compares\n", size,
                heap_stats.moves, heap_stats.compares);

            // Print x number of elements
            for (i = 0; i < elements; i++) {
                if ((i % 5 == 0) && (i != 0)) {
                    printf("\n");
                }
                printf("%12d", s_array[i]);
            }
            printf("\n");
        }
        if (graph == true) {
            printf("%d\t%" PRIu64, size, heap_stats.moves);
            printf("\n");
        }
        free(s_array);
    }
    if (set_member(flags, HELP)) {
        printf("SYNOPSIS\n  A collection of comparison-based sorting algorithms.\n\nUSAGE\n  "
               "./sorting [ -Hasbhqn:p:r: ] [ -n length ] [ -p elements ] [ -r seed ]\n\nOPTIONS\n "
               " -H\t\tDisplay program help and usage\n  -a\t\tEnable all sorts.\n  -s\t\tEnable "
               "Shell Sort.\n  -b\t\tEnable Batcher Sort.\n  -h\t\tEnable Heap Sort.\n  "
               "-q\t\tEnable Quick Sort.\n  -n length\tSpecify number of array elements (default: "
               "100).\n  -p elements\tSpecify number of elements to print (default: 100).\n  -r "
               "seed\tSpecify random seed (default: 13371453).\n");
    }

    // PRINT ARRAY TEST
    //for (i = 0; i < size; i++)
    //{
    //	printf("%d\n", s_array[i]);
    //}

    clock_t difference = (clock() - before) / CLOCKS_PER_SEC;

    if (flags != set_empty())
    {
        printf("%ld seconds\n", difference);
    }
    return 0;
}
