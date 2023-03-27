# Calculating e, pi, & square roots  

## How to build, run, and clean
```
$ make all
$ ./mathlib-test {args}
$ make clean
$ make cleangraph
```

## How to use plotting script
```
$ make all
$ chmod +x plot.sh
$ ./plot.sh
```

### Source files
* mathlib-test.c
        - Test bench for calling functions
* mathlib.h
        - Function declarations
* Makefile
        - Create executable "mathlib-test.c" and object
* e.c
        - Euler's number approximation
* bbp.c
        - Pi approximation using Bailey-Borwein-Plouffe
* madhava.c
        - Pi approximation using Madhava series
* euler.c
        - Pi approximation using Euler's formula
* newton.c
        - Square root approximation given input value
* viete.c
        - Pi approximation using Viete's formula

### Misc.
* DESIGN.pdf
* WRITEUP.pdf
* README.md
* plot.sh
* e.pdf
* bbp.pdf
* madhava.pdf
* euler.pdf
* sqrt.pdf
* viete.pdf
