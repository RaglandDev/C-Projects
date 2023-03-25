#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "trie.h"
#include "code.h"
#include "word.h"
#include "io.h"

#define OPTIONS      "vi:o:h"
#define BITLENGTH(x) floor(log2(x) + 1)

int main(int argc, char **argv) {
    // Open files (default stdin & stdout)
    int fd_infile = STDIN_FILENO;
    int fd_outfile = STDOUT_FILENO;

    // Parse options
    bool verbose = false;
    bool help = false;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'i': fd_infile = open(optarg, O_RDONLY); break;
        case 'o': fd_outfile = open(optarg, O_RDWR | O_CREAT); break;
        case 'h': help = true; break;
        }
    }

    // Error handle file descriptors
    if (fd_infile < 0) {
        perror("Error opening infile in main()\n");
        return -1;
    }
    if (fd_outfile < 0) {
        perror("Error opening outfile in main()\n");
        return -1;
    }

    // Initialize header, stats
    FileHeader in = { 0, 0 };
    struct stat stats_in;

    // Set outfile permissions to those from infile and get file size
    fstat(fd_infile, &stats_in);
    fchmod(fd_outfile, stats_in.st_mode);

    read_header(fd_infile, &in);

    // Decompress
    WordTable *table;
    uint8_t curr_sym;
    uint16_t curr_code, next_code;

    table = wt_create();
    curr_sym = 0;
    curr_code = 0;
    next_code = START_CODE;
    while (read_pair(fd_infile, &curr_code, &curr_sym, BITLENGTH(next_code)) == true) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(fd_outfile, table[next_code]);
        next_code = next_code + 1;
        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }
    flush_words(fd_outfile);
    wt_delete(table);

    // Set decompressed file size
    struct stat stats_out;
    fstat(fd_outfile, &stats_out);

    if (verbose) {
        printf("Compressed file size: %ld bytes\n", stats_in.st_size);
        printf("Decompressed file size: %ld bytes\n", stats_out.st_size);
        double space_saving = 100 * (1 - ((double) stats_in.st_size) / (double) stats_out.st_size);
        printf("Space saving: %lf%%\n", space_saving);
    }

    if (help) {
        printf("DECODE\n\nOptions\n -v Enables statistic printing\n -h Prints help message\n -i "
               "[input file (default stdin)]\n -o [output file (default stdout)]\n");
    }

    close(fd_infile);
    close(fd_outfile);
}
