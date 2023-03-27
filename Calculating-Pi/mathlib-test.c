#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

// Custom math library
#include "bbp.c"
#include "e.c"
#include "euler.c"
#include "madhava.c"
#include "mathlib.h"
#include "newton.c"
#include "viete.c"
//

#define OPTIONS "aebmrvnshg"

int main(int argc, char **argv) {
    int opt = 0;
    // flags to enable functions
    bool stats = false;
    bool all = false;
    bool help = false;
    bool ebool = false;
    bool bbpbool = false;
    bool madhavabool = false;
    bool eulerbool = false;
    bool vietebool = false;
    bool newtonbool = false;
    bool graph = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {

        // argument cases set function flags
        switch (opt) {
        case 's': stats = true; break;
        case 'a': all = true; break;
        case 'e': ebool = true; break;
        case 'b': bbpbool = true; break;
        case 'm': madhavabool = true; break;
        case 'r': eulerbool = true; break;
        case 'v': vietebool = true; break;
        case 'n': newtonbool = true; break;
        case 'h': help = true; break;
        case 'g': graph = true; break;
        }
    }

    // set all flags to true
    if (all == true) {
        ebool = true;
        bbpbool = true;
        madhavabool = true;
        eulerbool = true;
        vietebool = true;
        newtonbool = true;
    }

    // if flag for e function is set, execute it
    if (ebool == true) {
        double e_copy = e(graph);
        printf(
            "e() = %16.15lf, M_E =  %16.15lf, diff =  %16.15lf\n", e_copy, M_E, fabs(e_copy - M_E));
        if (stats == true) {
            printf("e() terms = %d\n", e_terms());
        }
    }

    if (bbpbool == true) {
        double bbp_copy = pi_bbp(graph);
        printf("pi_bbp() = %16.15lf, M_PI =  %16.15lf, diff =  %16.15lf\n", bbp_copy, M_PI,
            fabs(bbp_copy - M_PI));
        if (stats == true) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
        }
    }

    if (madhavabool == true) {
        double madhava_copy = pi_madhava(graph);
        printf("pi_madhava() = %16.15lf, M_PI =  %16.15lf, diff =  %16.15lf\n", madhava_copy, M_PI,
            fabs(madhava_copy - M_PI));
        if (stats == true) {
            printf("pi_madhava() terms = %d\n", madhava_pi_terms());
        }
    }

    if (eulerbool == true) {
        double euler_copy = pi_euler(graph);
        printf("pi_euler() = %16.15lf, M_PI =  %16.15lf, diff =  %16.15lf\n", euler_copy, M_PI,
            fabs(euler_copy - M_PI));
        if (stats == true) {
            printf("pi_euler() terms = %d\n", pi_euler_terms());
        }
    }

    if (vietebool == true) {
        double viete_copy = pi_viete(graph);
        printf("pi_viete() = %16.15lf, M_PI =  %16.15lf, diff =  %16.15lf\n", viete_copy, M_PI,
            fabs(viete_copy - M_PI));
        if (stats == true) {
            printf("pi_viete() terms = %d\n", pi_viete_terms());
        }
    }

    if (help == true) {
        printf("SYNOPSIS\n  A test harness for the small numerical library.\n\nUSAGE\n  "
               "./mathlib-test [-aebmrvnsh]\n\nOPTIONS\n  -a  Runs all tests.\n  -e  Runs e "
               "test.\n  -b  Runs BBP pi test.\n  -m  Runs Madhava pi test.\n  -r  Runs Euler pi "
               "test.\n  -v  Runs Viete pi test.\n  -n  Runs Newton square root tests.\n  -s  "
               "Print verbose statistics.\n  -h  Display program synopsis and usage.\n");
    }

    // newton flag executes tests on floats [0,10) and compares with library
    if (newtonbool == true) {
        for (double nti = 0; nti < 10; nti += .1) {
            double app = sqrt_newton(nti);
            double lapp = sqrt(nti);
            if (graph == true) {
                printf("%16.15lf\t%16.15lf\t%7.6lf\n", lapp, app, nti);
            } else {
                printf(
                    "sqrt_newton(%7.6lf) = %16.15lf, sqrt(%7.6lf) =  %16.15lf, diff =  %16.15lf\n",
                    nti, app, nti, lapp, fabs(lapp - app));
            }
            if (stats == true) {
                printf("sqrt_newton() terms = %d\n", sqrt_newton_iters());
            }
        }
    }

    return 0;
}
