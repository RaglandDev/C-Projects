#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int e_i = 0;

double e(bool graph) {
    int k = 0;
    double e_sum = 0;
    double k_fact = 1;
    double k_term = 1;

    while (k_term > EPSILON) {
        if (k > 0) {
            k_fact *= k;
        }

        k_term = 1 / k_fact;
        e_sum += k_term;
        k += 1;
        e_i++;
        if (graph == true) {
            printf("%16.15lf\t%d\n", e_sum, k);
        }
    }
    return e_sum;
}
int e_terms(void) {
    return (e_i);
}
