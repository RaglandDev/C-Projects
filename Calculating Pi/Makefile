CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
OBJECTS = mathlib-test.o bbp.o euler.o madhava.o e.o viete.o
CFILES = mathlib-test.c bbp.c euler.c madhava.c e.c viete.c
all: mathlib-test e.txt bbp.txt euler.txt madhava.txt viete.txt sqrt.txt

mathlib-test: $(OBJECTS)
	$(CC) -lm -o mathlib-test mathlib-test.o

mathlib-test.o: mathlib-test.c
	$(CC) $(CFLAGS) -c mathlib-test.c

bbp.o: bbp.c
	$(CC) $(CFLAGS) -c bbp.c

euler.o: euler.c
	$(CC) $(CFLAGS) -c euler.c

madhava.o: madhava.c
	$(CC) $(CFLAGS) -c madhava.c

e.o: e.c
	$(CC) $(CFLAGS) -c e.c

viete.o: viete.c
	$(CC) $(CFLAGS) -c viete.c



e.txt: mathlib-test
	./mathlib-test -g -e > e.txt
bbp.txt: mathlib-test
	./mathlib-test -g -b > bbp.txt
euler.txt: mathlib-test
	./mathlib-test -g -r > euler.txt
madhava.txt: mathlib-test
	./mathlib-test -g -m > madhava.txt
viete.txt: mathlib-test
	./mathlib-test -g -v > viete.txt
sqrt.txt: mathlib-test
	./mathlib-test -g -n > sqrt.txt

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -f mathlib-test *.o *.txt 
