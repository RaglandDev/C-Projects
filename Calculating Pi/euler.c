#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int euler_i = 0;

double pi_euler(bool graph) {
    double k = 1;
    double euler_sum = 0;
    double k_term = 1;

    while (k_term > EPSILON) {
        euler_sum += k_term;
        k += 1;
        k_term = 1 / (k * k);
        if (k_term < 0) {
            k_term = -k_term;
        }
        euler_i++;
        if ((graph == true) && (euler_i < 1000)) {
            printf("%16.15lf\t%d\n", sqrt_newton(euler_sum * 6), euler_i);
        }
    }
    euler_i++;
    euler_sum += k_term;
    return sqrt_newton(euler_sum * 6);
}

int pi_euler_terms(void) {
    return euler_i;
}
