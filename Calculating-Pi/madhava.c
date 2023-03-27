#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int madhava_i = 0;

// Returns value of pi calculated using Madhava series
// Checks graph parameter. If graph is true, prints output
double pi_madhava(bool graph) {
    double k = 0;
    double madhava_sum = 0;
    double k_term = 1;
    double POW = 1;
    while (k_term > EPSILON) {
        k_term = (1 / POW) / (2 * k + 1);
        madhava_sum += k_term;
        k += 1;
        POW *= (-3);
        madhava_i++;
        if (k_term < 0) {
            k_term = -k_term;
        }
        if (graph == true) {
            printf("%16.15lf\t%d\n", sqrt_newton(12) * madhava_sum, madhava_i);
        }
    }
    return (sqrt_newton(12) * madhava_sum);
}

int madhava_pi_terms(void) {
    return madhava_i;
}
