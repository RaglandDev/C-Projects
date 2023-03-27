#include "mathlib.h"

#include <stdio.h>
#include <stdlib.h>

static int sqrt_i = 0;

double sqrt_newton(double x) {
    sqrt_i = 0;
    double guess = x;
    double checker = (x - guess * guess);
    if (checker < 0) {
        checker = -checker;
    }
    while (checker > EPSILON) {
        guess = (guess + (x / guess)) / 2;
        checker = (x - guess * guess);
        if (checker < 0) {
            checker = -checker;
        }
        sqrt_i++;
    }
    return guess;
}
int sqrt_newton_iters(void) {
    return sqrt_i;
}
