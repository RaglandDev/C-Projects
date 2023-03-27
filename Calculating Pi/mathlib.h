#pragma once
#include <stdbool.h>
#define EPSILON 1e-14

static inline double absolute(double x) {
    return x < 0.0 ? -x : x;
}

double e(bool graph);

int e_terms(void);

double pi_euler(bool graph);

int pi_euler_terms(void);

double pi_bbp(bool graph);

int pi_bbp_terms(void);

double pi_madhava(bool graph);

int pi_madhava_terms(void);

double pi_viete(bool graph);

int pi_viete_factors(void);

double sqrt_newton(double);

int sqrt_newton_iters(void);
