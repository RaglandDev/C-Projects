CC = clang-15
CFLAGS = -Wall -Wextra -Werror -Wpedantic
OBJECTS = life.o universe.o
LINKS = -lncurses

all: life life.o universe.o

life: $(OBJECTS)
	$(CC) -o life $(OBJECTS) $(LINKS)

life.o: life.c 
	$(CC) $(CFLAGS) -c life.c 

universe.o: universe.c 
	$(CC) $(CFLAGS) -c universe.c 

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -f life *.o 
