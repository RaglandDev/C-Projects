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
        case 'h': help = true; break;
        case 'i': fd_infile = open(optarg, O_RDONLY); break;
        case 'o': fd_outfile = open(optarg, O_RDWR | O_CREAT); break;
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
    FileHeader out = { 0, 0 };
    struct stat stats_in;

    // Set outfile permissions to those from infile and get file size
    fstat(fd_infile, &stats_in);
    fchmod(fd_outfile, stats_in.st_mode);

    // Set magic number and protection bits to outfile header
    out.magic = MAGIC;
    out.protection = stats_in.st_mode;

    write_header(fd_outfile, &out);

    // COMPRESSION
    TrieNode *root, *curr_node, *prev_node, *next_node;
    root = trie_create();
    curr_node = root;
    prev_node = NULL;
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_sym(fd_infile, &curr_sym) == true) {
        next_node = trie_step(curr_node, curr_sym);
        if (next_node != NULL) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(fd_outfile, curr_node->code, curr_sym, BITLENGTH(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code = next_code + 1;
        }
        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }
        prev_sym = curr_sym;
    }
    if (curr_node != root) {
        write_pair(fd_outfile, prev_node->code, prev_sym, BITLENGTH(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }
    write_pair(fd_outfile, STOP_CODE, 0, BITLENGTH(next_code));
    flush_pairs(fd_outfile);
    trie_delete(root);

    // Set compressed file size
    struct stat stats_out;
    fstat(fd_outfile, &stats_out);

    if (verbose) {
        printf("Uncompressed file size: %ld\n", stats_in.st_size);
        printf("Compressed file size: %ld\n", stats_out.st_size);
        double space_saving = 100 * (1 - ((double) stats_out.st_size / (double) stats_in.st_size));
        printf("Space saving: %lf%%\n", space_saving);
    }

    if (help) {
        printf("ENCODE\n\nOptions\n -v Enables statistic printing\n -h Prints help message\n -i "
               "[input file (default stdin)]\n -o [output file (default stdout)]\n");
    }

    close(fd_outfile);
    close(fd_infile);
    return 0;
}
