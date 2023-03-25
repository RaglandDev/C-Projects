#include "io.h"
#include "endian.h"
#include "code.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

// Initialize buffers & indexes
static uint8_t sym_buf[BLOCK];
static uint8_t bit_buf[BLOCK];
static int sym_buf_index = 0;
static int bit_buf_index = 0;

uint64_t total_syms;
uint64_t total_bits;

//
// Read up to to_read bytes from infile and store them in buf. Return the number of bytes actually
// read.
//
// Since read() may not read in as many bytes as you asked for, this function should continuously
// call read() and attempt to read as many bytes as it has not yet read. For instance, if to_read is
// 100 and the first read() call only reads 20 bytes, it should attempt to read 80 bytes the next
// time it calls read().
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int byte_count = 0;
    int bytes_read = 0;
    uint8_t *buf_t = buf;
    int to_read_t = to_read;

    do {
        bytes_read = read(infile, buf_t, to_read_t);
        if (bytes_read == 0) {
            break;
        } else if (bytes_read < 0) {
            perror("Error reading bytes in read_bytes()\n");
            return -1;
        } else {
            byte_count += bytes_read;
            buf_t += bytes_read;
            to_read_t -= bytes_read;
        }
    } while (byte_count < to_read_t);

    return byte_count;
}

//
// Write up to to_write bytes from buf into outfile. Return the number of bytes actually written.
//
// Similarly to read_bytes, this function will need to call write() in a loop to ensure that it
// writes as many bytes as possible.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int byte_count = 0;
    int bytes_written = 0;
    uint8_t *buf_t = buf;
    int to_write_t = to_write;

    do {
        bytes_written = write(outfile, buf_t, to_write_t);
        if (bytes_written == 0) {
            break;
        } else if (bytes_written < 0) {
            perror("Error writing bytes in write_bytes()\n");
            return -1;
        } else {
            byte_count += bytes_written;
            buf_t += bytes_written;
            to_write_t -= bytes_written;
        }

    } while (byte_count < to_write_t);

    return byte_count;
}

//
// Read a file header from infile into *header.
//
// This function need not create any additional buffer to store the contents of the file header.
// File headers, like any struct or any value in C, are just represented by bytes in memory which
// means you can use read_bytes to read however many bytes the header consumes (use sizeof!) into
// *header.
//
// Since we decided that the canonical byte order for our headers is little-endian, this function
// will need to swap the byte order of both the header fields if it is run on a big-endian system.
// For example, here is how the 4 bytes of the magic number will look when written to the file:
//
// +------+------+------+------+
// | 0xAC | 0xBA | 0xAD | 0xBA |
// +------+------+------+------+
//
// A big-endian computer would interpret those bytes as the 4-byte number 0xACBAADBA, which is
// not what we want, so you would have to change the order of those bytes in memory. A little-endian
// computer will interpret that as 0xBAADBAAC.
//
// This function should also make sure the magic number is correct. Since it has no return value you
// may call assert() to do that, or print out an error message and exit the program, or use some
// other way to report the error.
//
void read_header(int infile, FileHeader *header) {
    read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));

    // Check magic number
    if (header->magic != MAGIC) {
        perror("Magic number not found in read_header()\n");
        return;
    }

    // Check endian-ness, swap bytes if necessary
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }
}

//
// Write a file header from *header to outfile. Like above, this function should swap the byte order
// of the header's two fields if necessary.
//
void write_header(int outfile, FileHeader *header) {
    // Check endian, swap bytes if necessary
    if (big_endian()) {
        header->magic = swap32(header->magic);
        header->protection = swap16(header->protection);
    }

    // Write bytes from header struct to outfile
    write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

//
// Read one symbol from infile into *sym. Return true if a symbol was successfully read, false
// otherwise.
//
// Reading one symbol at a time is slow, so this function will need to maintain a global buffer
// (an array) of BLOCK bytes. Most calls will only need to read a symbol out of that buffer, and
// then update some counter so that the function knows what position in the buffer it is at. If
// there are no more bytes in the buffer for it to return, it will have to call read_bytes to refill
// the buffer with fresh data. If this call fails then you cannot read a symbol and should return
// false.
//
bool read_sym(int infile, uint8_t *sym) {
    int bytes_read = 0;

    // Fill buffer (initially, and at end of buffer)
    if ((sym_buf_index == BLOCK) || (sym_buf_index == 0)) {
        memset(sym_buf, 0, sizeof(sym_buf));
        bytes_read = read_bytes(infile, sym_buf, BLOCK);

        // No more bytes to read or error
        if (bytes_read == 0) {
            sym_buf_index = 0;
            return false;
        } else if (bytes_read < 0) {
            perror("Error reading bytes in read_sym()\n");
            return false;
        }
        sym_buf_index = 0;
    }

    // Process symbol from buffer
    *sym = sym_buf[sym_buf_index];
    sym_buf_index++;
    return true;
}

//
// Write a pair -- bitlen bits of code, followed by all 8 bits of sym -- to outfile.
//
// This function should also use a buffer. It writes into individual bits in the buffer, starting
// with the least significant bit of the first byte, until the most significant bit of the first
// byte, and then the least significant bit of the second byte, and so on. You will need bitwise
// arithmetic to manipulate individual bits within your buffer -- consult asgn3/set.c if you don't
// remember how to do this.
//
// The first bit of code to be written is the least significant bit, and the same holds for sym.
//
// This function will need to track which *bit* in the buffer will be written to next. If it ever
// reaches the end of the buffer it needs to write out the contents of the buffer to outfile; you
// may use flush_pairs to do this.
//
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    // Write code bits
    for (uint8_t i = 0; i < bitlen; i++) {
        // Flush pairs at end of buffer
        if (bit_buf_index == BLOCK) {
            flush_pairs(outfile);
        }

        uint8_t bit_position = bit_buf_index % 8;
        uint8_t bit = (code >> i) & 1;
        bit_buf[bit_buf_index / 8] |= bit << bit_position;
        bit_buf_index++;
    }

    // Write symbol bits
    for (uint8_t i = 0; i < 8; i++) {
        // Flush pairs at end of buffer
        if (bit_buf_index == BLOCK) {
            flush_pairs(outfile);
        }

        uint8_t bit_position = bit_buf_index % 8;
        uint8_t bit = (sym >> i) & 1;
        bit_buf[bit_buf_index / 8] |= bit << bit_position;
        bit_buf_index++;
    }
}

//
// Read bitlen bits of a code into *code, and then a full 8-bit symbol into *sym, from infile.
// Return true if the complete pair was read and false otherwise.
//
// Like write_pair, this function must read the least significant bit of each input byte first, and
// will store those bits into the LSB of *code and of *sym first.
//
// It may be useful to write a helper function that reads a single bit from a file using a buffer.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    // Buffer is empty
    if (bit_buf_index == 0) {
        memset(bit_buf, 0, sizeof(bit_buf));
        int bytes_read = read_bytes(infile, bit_buf, BLOCK);

        if (bytes_read == 0) {
            return false;
        } else if (bytes_read < 0) {
            perror("Error reading bytes in read_pair()\n");
        }
    }

    // Read code (from LSB)
    *code = 0;
    for (int i = 0; i < bitlen; i++) {
        if (bit_buf_index == BLOCK * 8) {
            memset(bit_buf, 0, sizeof(bit_buf));
            bit_buf_index = 0;

            int bytes_read = read_bytes(infile, bit_buf, BLOCK);
            if (bytes_read == 0) {
                return false;
            } else if (bytes_read < 0) {
                perror("Error reading bytes in read_pair()\n");
                return false;
            }
        }
        // Byte we are checking
        uint8_t current_byte = bit_buf[bit_buf_index / 8];

        // Creates a mask with a bit at the position we want to check
        uint8_t mask = (1 << (bit_buf_index % 8));

        // Checks if the bit at bit_buf_index position is set in bit_buf
        if ((current_byte & mask) != 0) {
            *code |= (1 << i);
        }
        bit_buf_index++;
    }

    // Read symbol (from LSB)
    *sym = 0;
    for (int i = 0; i < 8; i++) {
        if (bit_buf_index == BLOCK * 8) {
            memset(bit_buf, 0, sizeof(bit_buf));
            bit_buf_index = 0;

            int bytes_read = read_bytes(infile, bit_buf, BLOCK);
            if (bytes_read == 0) {
                return false;
            } else if (bytes_read < 0) {
                perror("Error reading bytes in read_pair()\n");
                return false;
            }
        }

        // Byte we are checking
        uint8_t current_byte = bit_buf[bit_buf_index / 8];

        // Creates a mask with a bit at the position we want to check
        uint8_t mask = (1 << (bit_buf_index % 8));

        // Checks if the bit at bit_buf_index position is set in bit_buf
        if ((current_byte & mask) != 0) {
            *sym |= (1 << i);
        }
        bit_buf_index++;
    }

    if (*code == STOP_CODE) {
        return false;
    } else {
        return true;
    }
}

//
// Write any pairs that are in write_pair's buffer but haven't been written yet to outfile.
//
// This function will need to be called at the end of encode since otherwise those pairs would never
// be written. You don't have to, but it would be easy to make this function also work when called
// by write_pair, since otherwise you would write essentially the same code in two places.
//
// If not all bits of the last byte in your buffer have been written to, make sure that the
// unwritten bits are set to zero. An easy way to do this is by zeroing the entire buffer after
// flushing it every time.
//
void flush_pairs(int outfile) {
    write_bytes(outfile, bit_buf, bit_buf_index / 8);
    bit_buf_index = 0;

    // Zero array
    memset(bit_buf, 0, sizeof(bit_buf));
}

//
// Write every symbol from w into outfile.
//
// These symbols should also be buffered and the buffer flushed whenever necessary (note you will
// likely sometimes fill up your buffer in the middle of writing a word, so you cannot only check
// that the buffer is full at the end of this function).
//
void write_word(int outfile, Word *w) {
    for (uint32_t i = 0; i < w->len; i++) {
        if (w->syms[i] != 0) {
            sym_buf[sym_buf_index] = w->syms[i];
            sym_buf_index++;

            // Buffer fills up
            if (sym_buf_index == BLOCK) {
                flush_words(outfile);
                ;
            }
        }
    }
}

//
// Write any unwritten word symbols from the buffer used by write_word to outfile.
//
// Similarly to flush_pairs, this function must be called at the end of decode since otherwise you
// would have symbols remaining in the buffer that were never written.
//
void flush_words(int outfile) {
    write_bytes(outfile, sym_buf, sym_buf_index);
    sym_buf_index = 0;

    // Zero array
    memset(sym_buf, 0, sizeof(sym_buf));
}
