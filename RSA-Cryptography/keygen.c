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

#define OPTIONS "b:i:n:d:s:vh"

int main(int argc, char **argv) {
    // DEFAULTS
    uint64_t bits = 256;
    uint64_t iters = 50;

    char pub_name[128 + 1] = "ss.pub";
    char priv_name[128 + 1] = "ss.priv";

    uint64_t seed = time(NULL);

    bool verbose = false;
    bool help = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': bits = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'i': iters = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'n':
            memset(pub_name, 0, 128 * sizeof(char));
            snprintf(pub_name, 128, "%s", optarg);
            break;
        case 'd':
            memset(priv_name, 0, 128 * sizeof(char));
            snprintf(pub_name, 128, "%s", optarg);
            break;
        case 's': seed = (uint32_t) strtoul(optarg, NULL, 10); break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        }
    }

    if (help) {
        printf("SYNOPSIS\n  Generates an SS public/private key pair.\n\nUSAGE\n./keygen "
               "[OPTIONS]\n\nOPTIONS\n  -h\t\tDisplay program help and usage.\n  -v\t\tDisplay "
               "verbose program output.\n  -b bits\tMinimum bits needed for public key n (default: "
               "256).\n  -i iterations\tMiller-Rabin iterations for testing primes (default: "
               "50).\n  -n pbfile\tPublic key file (default: ss.pub).\n  -d pvfile\tPrivate key "
               "file (default: ss.priv).\n  -s seed\tRandom seed for testing.\n");
    }

    // Open key files
    FILE *pub_fp = fopen(pub_name, "w");
    FILE *priv_fp = fopen(priv_name, "w");
    if ((!pub_fp) || (!priv_fp)) {
        perror("Could not open file for writing\n");
        return EXIT_FAILURE;
    }

    // Set private file permissions read & write
    int priv_d = open(priv_name, O_RDWR);
    fchmod(priv_d, S_IRUSR | S_IWUSR);

    // Initialize random state
    randstate_init(seed);
    srandom(seed);

    // Make public & private keys
    mpz_t p, q, n, pq, d;
    mpz_inits(p, q, n, pq, d, NULL);

    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, n, p, q);
    mpz_mul(pq, p, q);

    // Get current username
    char username[128 + 1];
    snprintf(username, 128, "%s", getenv("USER"));

    // Write computed private & public keys to respective files
    ss_write_pub(n, username, pub_fp);
    ss_write_priv(pq, d, priv_fp);

    if (verbose) {
        gmp_printf("%s\n"
                   "%Zd (%d bits)\n"
                   "%Zd (%d bits)\n"
                   "%Zd (%d bits)\n"
                   "%Zd (%d bits)\n"
                   "%Zd (%d bits)\n",
            username, p, mpz_sizeinbase(p, 2), q, mpz_sizeinbase(q, 2), n, mpz_sizeinbase(n, 2), d,
            mpz_sizeinbase(d, 2), pq, mpz_sizeinbase(pq, 2));
    }

    // Close files, clear randomstate, clear mpz variables
    fclose(pub_fp);
    fclose(priv_fp);
    randstate_clear();
    mpz_clears(p, q, n, pq, d, NULL);

    return 0;
}
/*
    //
    // TEST BENCH
    randstate_init(100);
    srandom(100);
    mpz_t a, b, c, d, pq;
    mpz_inits(a, b, c, d, pq, NULL);
    ss_make_pub(a, b, c, 6000, 100);
    ss_make_priv(d, c, a, b);
    mpz_mul(pq, a, b);

    gmp_printf("public key = %Zx\n\nprivate key = %Zx\n", c, d);
    printf("\n");

    // TEST ENCRYPT / DECRYPT
    mpz_t encrypted_integer, original_integer, decrypted_integer;
    mpz_inits(encrypted_integer, original_integer, decrypted_integer, NULL);
    mpz_set_ui(original_integer, 69);
    ss_encrypt(encrypted_integer, original_integer, c);
    ss_decrypt(decrypted_integer, encrypted_integer, d, pq);
    gmp_printf("Original integer...%Zd\nEncrypted integer...%Zd\nDecrypted integer...%Zd\n",
        original_integer, encrypted_integer, decrypted_integer);
    mpz_clears(encrypted_integer, original_integer, decrypted_integer, NULL);

    // TEST WRITE PUBLIC KEY
    char user[] = { "user" };
    FILE *output = fopen("public.txt", "w");
    ss_write_pub(c, user, output);
    fclose(output);
    printf("\n");

    // TEST WRITE PRIVATE KEY
    FILE *output2 = fopen("private.txt", "w");
    ss_write_priv(pq, d, output2);
    fclose(output2);
    printf("\n");

    // TEST READ PUBLIC KEY
    char pub_read[50];
    mpz_t n;
    mpz_init(n);
    FILE *output3 = fopen("public.txt", "r");
    ss_read_pub(n, pub_read, output3);
    gmp_printf("reading public key...%Zx\n", n);
    printf("reading username...");
    for (int i = 0; i < 5; i++) {
        printf("%c", pub_read[i]);
    }
    printf("\n");
    mpz_clear(n);
    fclose(output3);
    printf("\n");

    // TEST READ PRIVATE KEY
    mpz_t pq_test, d_test;
    mpz_init(pq_test);
    mpz_init(d_test);
    FILE *output4 = fopen("private.txt", "r");
    ss_read_priv(pq_test, d_test, output4);
    gmp_printf("reading private key...%Zx\n\n", d_test);
    gmp_printf("reading pq...%Zx\n", pq_test);
    mpz_clear(pq_test);
    mpz_clear(d_test);
    fclose(output4);

    // TEST ENCRYPT PRIVATE KEY
    FILE *input5 = fopen("private.txt", "r");
    FILE *output5 = fopen("encrypted_private.txt", "w");
    ss_encrypt_file(input5, output5, c);
    fclose(input5);
    fclose(output5);

    // TEST DECRYPT PRIVATE KEY
    FILE *input6 = fopen("encrypted_private.txt", "r");
    FILE *output6 = fopen("decrypted_private.txt", "w");
    ss_decrypt_file(input6, output6, d, pq);

    mpz_clears(a, b, c, pq, NULL);
    randstate_clear();
    // END TEST BENCH
    //
    */
