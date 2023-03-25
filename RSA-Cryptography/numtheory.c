#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    mpz_t t, tempa, tempb;
    mpz_inits(t, tempa, tempb, NULL);
    mpz_set(tempa, a);
    mpz_set(tempb, b);

    while (mpz_sgn(tempb) != 0) {
        mpz_set(t, tempb);
        mpz_mod(tempb, tempa, tempb);
        mpz_set(tempa, t);
    }
    mpz_set(g, tempa);

    mpz_clears(t, tempa, tempb, NULL);
}

void mod_inverse(mpz_t o, mpz_t const a, const mpz_t n) {
    mpz_t tempa, tempn, r, rp, t, tp, q, rexp, texp, f, g;
    mpz_inits(tempa, tempn, r, rp, t, tp, q, rexp, texp, f, g, NULL);
    mpz_set(tempa, a);
    mpz_set(tempn, n);

    mpz_set(r, tempn);
    mpz_set(rp, tempa);
    mpz_set_ui(t, 0);
    mpz_set_ui(tp, 1);

    // while rprime != 0
    while (mpz_cmp_ui(rp, 0) != 0) {
        // q = r/rp
        mpz_fdiv_q(q, r, rp);

        // Update r and rp
        mpz_set(rexp, r);
        mpz_set(r, rp);
        mpz_mul(f, q, rp);
        mpz_sub(rp, rexp, f);

        // Update t and tp
        mpz_set(rexp, t);
        mpz_set(t, tp);
        mpz_mul(g, q, tp);
        mpz_sub(tp, rexp, g);
    }

    // r > 1
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        mpz_clears(tempa, tempn, r, rp, t, tp, q, rexp, texp, f, g, NULL);
        return;
    }
    // t < 0
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, tempn);
    }

    mpz_set(o, t);
    mpz_clears(tempa, tempn, r, rp, t, tp, q, rexp, texp, f, g, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    mpz_t v, p, f, g, tempa, tempd, tempn;
    mpz_inits(v, p, f, g, tempa, tempd, tempn, NULL);
    mpz_set(tempa, a);
    mpz_set(tempd, d);
    mpz_set(tempn, n);

    // v = 1
    mpz_set_ui(v, 1);
    // p = a
    mpz_set(p, tempa);

    // while d > 0
    while (mpz_sgn(tempd) > 0) {
        // if odd(d)
        if (mpz_odd_p(tempd) != 0) {
            // v = (v * p) % n
            mpz_mul(f, v, p);
            mpz_mod(v, f, tempn);
        }
        // p = (p * p) % n
        mpz_mul(g, p, p);
        mpz_mod(p, g, tempn);
        // d = floor(d / 2)
        mpz_fdiv_q_ui(tempd, tempd, 2);
    }

    mpz_set(o, v);
    mpz_clears(v, p, f, g, tempa, tempd, tempn, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    mpz_t r, tempn, y, nmo, j, smo, randint, nmt, d;
    mpz_inits(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
    mpz_set(tempn, n);

    // Edge cases n = [0, 1, 2, 3], n = [evens]
    if (mpz_cmp_ui(tempn, 0) == 0) {
        mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
        return false;
    } else if (mpz_cmp_ui(tempn, 1) == 0) {
        mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
        return false;
    } else if (mpz_cmp_ui(tempn, 2) == 0) {
        mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
        return true;
    } else if (mpz_cmp_ui(tempn, 3) == 0) {
        mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
        return true;
    } else if (mpz_even_p(tempn)) {
        mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
        return false;
    }

    // r = n - 1
    mpz_sub_ui(r, tempn, 1);
    mp_bitcnt_t s = 0; // CITATION: VARUN TUTOR
    while (mpz_even_p(r)) {
        s++;
        mpz_tdiv_q_2exp(r, r, 1);
    }
    // END CITATION

    for (uint64_t i = 1; i <= iters; i++) {

        // nmt = n - 3
        mpz_sub_ui(nmt, tempn, 3);

        // choose random a [2, n - 2]
        // urandomm is [0, n - 1], so we input n-3 because we are going to add 2 to
        // increase the lower bound so we need to compensate the upper bound
        mpz_urandomm(randint, state, nmt);
        // randint = randint + 2
        mpz_add_ui(randint, randint, 2);

        // y = power-mod(a, r, n)
        pow_mod(y, randint, r, tempn);

        // nmo = n - 1
        mpz_sub_ui(nmo, tempn, 1);
        // if y != 1 AND y != n - 1
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, nmo) != 0)) {
            // j = 1
            mpz_set_ui(j, 1);

            // smo = s - 1
            mpz_set_ui(smo, (s - 1));
            // while (j <= (s - 1)) AND  (y != (n - 1))
            while ((((mpz_cmp(j, smo) < 0)) || (mpz_cmp(j, smo) == 0)) && (mpz_cmp(y, nmo) != 0)) {
                // d = 2
                mpz_set_ui(d, 2);
                // y = power-mod(y, 2, n)
                pow_mod(y, y, d, tempn);

                // if y == 1
                if ((mpz_cmp_ui(y, 1) == 0)) {
                    mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
                    return false;
                }
                // j = j + 1
                mpz_add_ui(j, j, 1);
            }
            // if y != n - 1
            if ((mpz_cmp(y, nmo) != 0)) {
                mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, tempn, y, nmo, j, smo, randint, nmt, d, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    do {
        mpz_urandomb(p, state, bits);
    } while (!is_prime(p, iters) || (mpz_sizeinbase(p, 2) < bits));
}
