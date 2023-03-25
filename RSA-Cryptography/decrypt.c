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

    char priv_name[128 + 1] = "ss.priv";

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
            memset(priv_name, 0, 128 * sizeof(char));
            snprintf(priv_name, 128, "%s", optarg);
            break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        }
    }

    if (help) {
        printf(
            "SYNOPSIS\n  Decrypts data using SS decryption.\n  Encrypted data is encrypted by the "
            "encrypt program.\n\nUSAGE\n  ./decrypt [OPTIONS]\n\nOPTIONS\n  -h\tDisplay program "
            "help and usage.\n  -v\tDisplay verbose program ooutput.\n  -i infile\tInput file of "
            "data to decrypt (default: stdin).\n  -o outfile\tOutput file for decrypted data "
            "(default: stdout).\n  -n pvfile\tPrivate key file (default: ss.priv).\n");
    }

    // Open pub key file
    FILE *priv_file = fopen(priv_name, "r");
    if (!priv_file) {
        perror("Could not open file for reading\n");
        return EXIT_FAILURE;
    }

    mpz_t pq, d;
    mpz_inits(pq, d, NULL);

    // Read private key
    ss_read_priv(pq, d, priv_file);

    if (verbose) {
        gmp_printf("Private modulus pq: %Zd (%d bits)\n"
                   "Private key: %Zd (%d bits)\n",
            pq, mpz_sizeinbase(pq, 2), d, mpz_sizeinbase(d, 2));
    }

    // Decrypt file
    ss_decrypt_file(input, output, d, pq);

    // Close public key file and clear any mpz_t variables
    fclose(input);
    fclose(output);
    fclose(priv_file);
    mpz_clears(pq, d, NULL);
    return 0;
}
