CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic 
OBJECTS = sorting.o batcher.o shell.o heap.o quick.o stats.o set.o

all: sorting 

sorting: $(OBJECTS)
	$(CC) -lm -o sorting sorting.o

sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c

batcher.o: batcher.c
	$(CC) $(CFLAGS) -c batcher.c

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c

quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c

stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c

set.o: set.c 
	$(CC) $(CFLAGS) -c set.c


clean:
	rm -f sorting *.o *.txt

cleangraph:
	rm -f all.pdf batcher.pdf heap.pdf quick.pdf shell.pdf

format:
	clang-format -i -style=file *.[ch]


