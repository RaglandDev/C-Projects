#include "ss.h"
#include "numtheory.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
//
// Generates the components for a new SS key.
//
// Provides:
//  p:  first prime
//  q: second prime
//  n: public modulus/exponent
//
// Requires:
//  nbits: minimum # of bits in n
//  iters: iterations of Miller-Rabin to use for primality check
//  all mpz_t arguments to be initialized
//
void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {
    // Generate random number [nbits/5, (2 * nbits) / 5]
    //
    // bounds = [nbits/5, (2 * nbits)/5]
    uint64_t lower = nbits / 5;
    uint64_t upper = (2 * nbits) / 5;
    uint64_t size = upper - lower + 1;
    // Generate random number within [0, size - 1]
    uint64_t p_bits = random() % size;
    // Change range to [lower, upper]
    p_bits += lower;

    // Calculate bits for q
    int64_t q_bits = nbits - (2 * p_bits);

    mpz_t prime_p, prime_q, qmo, pmo, temp_mod_a, temp_mod_b, prime_p_s;
    mpz_inits(prime_p, prime_q, qmo, pmo, prime_p_s, temp_mod_a, temp_mod_b, NULL);

    // Generate p & q, while checking p doesn't divide q - 1, and vice versa
    do {
        make_prime(prime_p, p_bits, iters);
        make_prime(prime_q, q_bits, iters);
        mpz_sub_ui(qmo, prime_q, 1);
        mpz_sub_ui(pmo, prime_p, 1);
        mpz_mod(temp_mod_a, prime_p, qmo);
        mpz_mod(temp_mod_b, prime_q, pmo);
        // while (((prime_p % qmo) == 0) && ((prime_q % pmo) == 0));
    } while ((mpz_cmp_ui(temp_mod_a, 0) == 0) && (mpz_cmp_ui(temp_mod_b, 0) == 0));

    mpz_set(p, prime_p);
    mpz_set(q, prime_q);

    mpz_mul(prime_p_s, prime_p, prime_p);
    mpz_mul(n, prime_p_s, prime_q);

    mpz_clears(prime_p, prime_q, qmo, pmo, temp_mod_a, temp_mod_b, prime_p_s, NULL);
}

//
// Generates components for a new SS private key.
//
// Provides:
//  d:  private exponent
//  pq: private modulus
//
// Requires:
//  p:  first prime number
//  q: second prime number
//  all mpz_t arguments to be initialized
//
void ss_make_priv(mpz_t d, const mpz_t n, const mpz_t p, const mpz_t q) {
    // n = p * p * q
    //
    // d = mod_inverse(d, n, lambda(pq))
    // lambda(pq) = lcm(p - 1, q - 1)
    // lcm(p - 1, q - 1) = ((p - 1) * (q - 1)) / gcd(p - 1, q - 1)
    // d = mod_inverse(d, (p - 1)(q - 1) / gcd(p - 1, q - 1))
    mpz_t pmo, qmo, pmo_x_qmo, rop, g;
    mpz_inits(pmo, qmo, pmo_x_qmo, g, rop, NULL);

    mpz_sub_ui(pmo, p, 1);
    mpz_sub_ui(qmo, q, 1);
    mpz_mul(pmo_x_qmo, pmo, qmo);
    gcd(g, pmo, qmo);
    mpz_fdiv_q(rop, pmo_x_qmo, g);

    mod_inverse(d, n, rop);

    mpz_clears(pmo, qmo, pmo_x_qmo, g, rop, NULL);
}

//
// Export SS public key to output stream
//
// Requires:
//  n: public modulus/exponent
//  username: login name of keyholder ($USER)
//  pbfile: open and writable file stream
//

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);

    for (int i = 0; username[i] != '\0'; i++) {
        gmp_fprintf(pbfile, "%c", username[i]);
    }
    gmp_fprintf(pbfile, "\n");
}

//
// Export SS private key to output stream
//
// Requires:
//  pq: private modulus
//  d:  private exponent
//  pvfile: open and writable file stream
//
void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

//
// Import SS public key from input stream
//
// Provides:
//  n: public modulus
//  username: $USER of the pubkey creator
//
// Requires:
//  pbfile: open and readable file stream
//  username: requires sufficient space
//  all mpz_t arguments to be initialized
//
void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {

    gmp_fscanf(pbfile, "%Zx\n%s\n", n, username);
}

//
// Import SS private key from input stream
//
// Provides:
//  pq: private modulus
//  d:  private exponent
//
// Requires:
//  pvfile: open and readable file stream
//  all mpz_t arguments to be initialized
//
void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", pq, d);
}

//
// Encrypt number m into number c
//
// Provides:
//  c: encrypted integer
//
// Requires:
//  m: original integer
//  n: public exponent/modulus
//  all mpz_t arguments to be initialized
//
void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    // c = (m^n) % n
    pow_mod(c, m, n, n);
}

//
// Encrypt an arbitrary file
//
// Provides:
//  fills outfile with the encrypted contents of infile
//
// Requires:
//  infile: open and readable file stream
//  outfile: open and writable file stream
//  n: public exponent and modulus
//
void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    mpz_t m;
    mpz_inits(m, NULL);
    // Calculate block size k
    // k = floor(log2(sqrt(n) - 1) / 8)
    uint64_t k = floor(((mpz_sizeinbase(n, 2) / 2) - 1) / 8);

    // Allocate block that holds k bytes
    uint8_t *block = (uint8_t *) malloc(k * sizeof(uint8_t));

    // Set zeroth byte of block to 0xff
    block[0] = 0xFF;

    // While there are still unprocessed bytes in infile
    while (!feof(infile) && !ferror(infile)) {
        // Read k - 1 bytes in from infile
        // Place bytes at index 1 of buffer
        size_t read = fread(&block[1], sizeof(uint8_t), k - 1, infile);

        // EOF
        if (read == 0) {
            break;
        }

        // Convert read bytes, including 0xFF, into mpz_t m
        mpz_import(m, k, 1, sizeof(uint8_t), 1, 0, block);

        // Encrypt m
        ss_encrypt(m, m, n);

        // Write encrypted number to outfile as a hexstring
        gmp_fprintf(outfile, "%Zx\n", m);
    }
    mpz_clears(m, NULL);
    free(block);
}

//
// Decrypt number c into number m
//
// Provides:
//  m: decrypted/original integer
//
// Requires:
//  c: encrypted integer
//  d: private exponent
//  pq: private modulus
//  all mpz_t arguments to be initialized
//
void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    // m = (c^d) % pq
    pow_mod(m, c, d, pq);
}

//
// Decrypt a file back into its original form.
//
// Provides:
//  fills outfile with the unencrypted data from infile
//
// Requires:
//  infile: open and readable file stream to encrypted data
//  outfile: open and writable file stream
//  d: private exponent
//  pq: private modulus
//
void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    mpz_t c, m;
    mpz_inits(c, m, NULL);

    // Calculate block size k
    uint64_t k = floor((mpz_sizeinbase(pq, 2) - 1) / 8);

    // Allocate block that holds k bytes
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));

    // Scan in hexstrings
    // Save hexstring to mpz_t c
    while (gmp_fscanf(infile, "%Zx\n", &c) != EOF) {
        // Decrypt c back into original value m
        ss_decrypt(m, c, d, pq);

        // Convert m into bytes, store in block
        //
        // Bytes converted
        size_t j = 0;
        // Convert bytes
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, m);

        // Write out j - 1 bytes starting from index 1
        for (size_t i = 1; i < j - 1; i++) {
            if (isalpha(block[i]) || isdigit(block[i])) {
                gmp_fprintf(outfile, "%c", block[i]);
            }
        }
    }

    mpz_clears(c, m, NULL);
    free(block);
}
