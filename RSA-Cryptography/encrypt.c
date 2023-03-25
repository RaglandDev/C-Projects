#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#define OPTIONS "vhi:n:o:"

int main(int argc, char **argv) {
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    FILE *input = stdin;
    FILE *output = stdout;

    char pub_name[128 + 1] = "ss.pub";

    bool verbose = false;
    bool help = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            input = fopen(optarg, "r");
            if (!input) {
                perror("Could not open file for reading\n");
                return EXIT_FAILURE;
            };
            break;
        case 'o':
            output = fopen(optarg, "w");
            if (!output) {
                perror("Could not open file for writing\n");
                return EXIT_FAILURE;
            };
            break;
        case 'n':
            memset(pub_name, 0, 128 * sizeof(char));
            snprintf(pub_name, 128, "%s", optarg);
            break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        }
    }

    if (help) {
        printf("SYNOPSIS\n  Encrypts data using SS encryption.\n  Encrypted data is decrypted by "
               "the decrypt program.\n\nUSAGE\n  ./encrypt [OPTIONS]\n\nOPTIONS\n  -h\tDisplay "
               "program help and usage.\n  -v\tDisplay verbose program output.\n  -i infile\tInput "
               "file of data to encrypt (default: stdin).\n  -o outfile\tOutput file for encrypted "
               "data (default: stdout).\n  -n pbfile\tPublic key file (default: ss.pub).\n");
    }

    // Open pub key file
    FILE *pub_file = fopen(pub_name, "r");
    if (!pub_file) {
        perror("Could not open file for reading\n");
        return EXIT_FAILURE;
    }

    char username[128 + 1];
    mpz_t n;
    mpz_inits(n, NULL);

    // Read public key from file
    ss_read_pub(n, username, pub_file);

    if (verbose) {
        gmp_printf("%s\n"
                   "%Zd (%d bits)\n",
            username, n, mpz_sizeinbase(n, 2));
    }

    // Encrypt file
    ss_encrypt_file(input, output, n);

    // Close public key file and clear any mpz_t variables
    fclose(input);
    fclose(pub_file);
    fclose(output);
    mpz_clears(n, NULL);
    return 0;
}
