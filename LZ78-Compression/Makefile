CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -Werror
SRC = $(wildcard *.c)
OBJ = io.o trie.o word.o
EXECBIN = encode decode
LFLAGS = -lm

.PHONY: all clean

all: $(EXECBIN)

clean:
	rm -f $(OBJ) $(EXECBIN) encode.o decode.o *.test
format:
	clang-format -i -style=file *.[ch]
	

encode: $(OBJ) encode.o
	$(CC) -o encode $(OBJ) encode.o $(LFLAGS) 

decode: $(OBJ) decode.o
	$(CC) -o decode $(OBJ) decode.o $(LFLAGS) 


$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $(SRC)

encode.o: encode.c 
	$(CC) $(CFLAGS) -c encode.C

decode.o: decode.c 
	$(CC) $(CFLAGS) -c decode.C
