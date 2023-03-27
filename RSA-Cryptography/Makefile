CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic `pkg-config --cflags gmp` 
OBJECTS = randstate.o numtheory.o ss.o 
LFLAGS = `pkg-config --libs gmp` -lm -lgmp

all: keygen encrypt decrypt

decrypt: $(OBJECTS) decrypt.o
	$(CC) -o decrypt $(OBJECTS) decrypt.o $(LFLAGS)
encrypt: $(OBJECTS) encrypt.o
	$(CC) -o encrypt $(OBJECTS) encrypt.o $(LFLAGS)
keygen: $(OBJECTS) keygen.o
	$(CC) -o keygen $(OBJECTS) keygen.o $(LFLAGS)


decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c 
encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c 
randstate.o: randstate.c
	$(CC) $(CFLAGS) -c randstate.c
numtheory.o: numtheory.c
	$(CC) $(CFLAGS) -c numtheory.c
keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c
ss.o: ss.c
	$(CC) $(CFLAGS) -c ss.c


format:
	clang-format -i -style=file *.[ch]

clean:
	rm -f *.o *.txt keygen encrypt decrypt *.priv *.pub
