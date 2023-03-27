#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int bbp_i = 0;

// Returns value of pi calculated using BBP
// Checks graph parameter. If graph is true, prints output
double pi_bbp(bool graph) {
    double k = 0;
    double POW = 1;
    double bbp_sum = 0;
    double a = 4 / (8 * k + 1);
    double b = 2 / (8 * k + 4);
    double c = 1 / (8 * k + 5);
    double d = 1 / (8 * k + 6);
    double k_term = (a - b - c - d) / POW;

    while (k_term > EPSILON) {
        bbp_sum += k_term;
        k += 1;
        a = 4 / (8 * k + 1);
        b = 2 / (8 * k + 4);
        c = 1 / (8 * k + 5);
        d = 1 / (8 * k + 6);
        POW *= 16;
        k_term = (a - b - c - d) / POW;
        if (k_term < 0) {
            k_term = -k_term;
        }
        bbp_i++;
        if (graph == true) {
            printf("%16.15lf\t%d\n", bbp_sum, bbp_i);
        }
    }
    bbp_sum += k_term;
    bbp_i++;
    return bbp_sum;
}
int pi_bbp_terms(void) {
    return bbp_i;
}
