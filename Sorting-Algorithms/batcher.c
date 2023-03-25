#include "batcher.h"

#include <math.h>

static uint32_t bit_l = 0;
void bit_length(uint32_t
        num) // CITATION: https://www.geeksforgeeks.org/python-program-to-covert-decimal-to-binary-number/
{
    if (num > 1) {
        bit_length(floor(num / 2));
    }
    bit_l++;
}

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    //if (A[x] > A[y])
    if (cmp(stats, A[x], A[y]) > 0) {
        swap(stats, &A[x], &A[y]);
    }
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    uint32_t t, p, q, r, d, i;

    if (n == 0) {
        return;
    }

    bit_length(n);
    t = bit_l;
    p = 1 << (t - 1);
    bit_l = 0;

    while (p > 0) {
        q = 1 << (t - 1);
        r = 0;
        d = p;

        while (d > 0) {
            for (i = 0; i < (n - d); i++) {
                if ((i & p) == r) {
                    comparator(stats, A, i, i + d);
                }
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
