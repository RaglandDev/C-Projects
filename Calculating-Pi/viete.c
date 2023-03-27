#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int viete_i = 0;

// Returns value of pi calculated using Viete formula
// Checks graph parameter. If graph is true, prints output
double pi_viete(bool graph) {
    double root = sqrt_newton(2);
    double base = 2;
    while ((2 / root) > 1) {
        base = base * (2 / root);
        root = sqrt_newton(2 + root);
        viete_i++;
        if (graph == true) {
            printf("%16.15lf\t%d\n", base, viete_i);
        }
    }
    return base;
}

int pi_viete_terms(void) {
    return viete_i;
}
