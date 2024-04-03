#include "io.h"

#include "code.h"

#include <stdio.h>
#include <unistd.h>

static uint8_t bit_buffer[BLOCK] = ""; // Buffer for the code/symbol pairs.
static int bit_index = 0; // Index variable for the bit buffer.
static uint8_t sym_buffer[BLOCK] = ""; // Buffer for the symbols.
static int sym_index = 0; // Index variable for the symbol buffer.
static int buffer_end = BLOCK; // Index variable of the end of symbol buffer.

//
// Code inspired by Eugene's 3/9/2021 section.
// Function to loop read() syscalls. Returns the number of bytes read.
//
// infile: The int to read from.
// buf: The buffer as a uint8_t pointer to read into.
// to_read: The int to specify the number of bytes to read.
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
    int total = 0;
    int bytes = 0;

    while (to_read != 0 && (bytes = read(infile, buf, to_read)) > 0) {
        total += bytes;
        to_read -= bytes;
        buf += bytes; // Increment buf pointer to number of bytes read to avoid overwriting bytes previously read into the buffer.
    }

    return total;
}

//
// Code inspired by Eugene's 3/9/2021 section.
// Function to loop write() syscalls. Returns the number of bytes written.
//
// outfile: The int to write to.
// buf: The buffer as a uint8_t pointer to write from.
// to_write: The int to specify the number of bytes to write.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int total = 0;
    int bytes = 0;

    while (to_write != 0 && (bytes = write(outfile, buf, to_write)) > 0) {
        total += bytes;
        to_write -= bytes;
        buf += bytes;
    }

    return total;
}

//
// Reads in sizeof(FileHeader) bytes from the input file into a FileHeader.
//
// infile: The int to read from.
// header: The FileHeader pointer to read into.
//
void read_header(int infile, FileHeader *header) {
    int b = read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));
}

//
// Writes in sizeof(FileHeader) bytes to the output file from a FileHeader.
//
// outfile: The int to write into.
// header: The FileHeader pointer to write from.
//
void write_header(int outfile, FileHeader *header) {
    int b = write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
}

//
// Code inspired by Eugene's 3/2/2021 section.
// Read a symbol from an input file.
// Update the end of buffer index.
// Return true if there are any more symbols to read.
//
// infile: The int to read from.
// sym: The uint8_t pointer to read into.
//
bool read_sym(int infile, uint8_t *sym) {
    if (sym_index == 0) {
        int n = read_bytes(infile, sym_buffer, BLOCK);

        if (n < BLOCK) {
            buffer_end = n;
        }
    }

    *sym = sym_buffer[sym_index];
    sym_index += 1;

    if (sym_index == BLOCK * 8) {
        sym_index = 0;
    }

    if (buffer_end == sym_index) {
        return false;
    }

    return true;
}

//
// Code inspired by Eugene's 3/5/2021 section.
// Returns the number of bytes needed to represent a given number of bits.
//
// bits: The int of bits.
//
int bytes(int bits) {
    if ((bits / 8) == 0 && bits >= 8) {
        return bits / 8;
    }

    return bits / 8 + 1;
}

//
// Set the ith bit of a 16 bit integer.
//
// code: The uint16_t data to set the bit in.
// i: The position of the bit to set in the data.
//
void set_16bit(uint16_t *code, int i) {
    uint16_t index = i % 16;
    uint16_t mask = 1 << index;

    *code |= mask;

    return;
}

//
// Clear the ith bit of a 16 bit integer.
//
// code: The uint16_t data to clear the bit in.
// i: The position of the bit to clear in the data.
//
void clear_16bit(uint16_t *code, int i) {
    uint16_t index = i % 16;
    uint16_t mask = ~(1 << index);

    *code &= mask;

    return;
}

//
// Return the ith bit of a 16 bit integer.
//
// code: The uint16_t data to clear to get the bit from.
// i: The position of the bit to get in the data.
//
uint16_t get_16bit(uint16_t *code, int i) {
    uint16_t index = i % 16;
    uint16_t mask = 1 << index;

    uint16_t result = *code & mask;

    result >>= index;

    return result;
}

//
// Set the ith bit of an 8 bit integer.
//
// sym: The uint8_t data to set the bit in.
// i: The position of the bit to set in the data.
//
void set_8bit(uint8_t *sym, int i) {
    uint8_t index = i % 8;
    uint8_t mask = 1 << index;

    *sym |= mask;

    return;
}

//
// Clear the ith bit of an 8 bit integer.
//
// sym: The uint8_t data to clear the bit in.
// i: The position of the bit to clear in the data.
//
void clear_8bit(uint8_t *sym, int i) {
    uint8_t index = i % 8;
    uint8_t mask = ~(1 << index);

    *sym &= mask;

    return;
}

//
// Return the ith bit of an 8 bit integer.
//
// code: The uint8_t data to clear to get the bit from.
// i: The position of the bit to get in the data.
//
uint8_t get_8bit(uint8_t *sym, int i) {
    uint8_t index = i % 8;
    uint8_t mask = 1 << index;

    uint8_t result = *sym & mask;

    result >>= index;

    return result;
}

//
// Code inspired by Eugene's 3/2/2021 section.
// Write a buffered pair of code and symbol to the output file.
//
// outfile: The int to write to.
// code: The uint16_t code to write.
// sym: The uint8_t symbol to write.
// bitlen: The int of bits to buffer the code by.
//
void write_pair(int outfile, uint16_t code, uint8_t sym, int bitlen) {
    for (int i = 0; i < bitlen; i += 1) {
        if (get_16bit(&code, i) == 1) {
            // Set bit in the buffer.
            set_8bit(&bit_buffer[bytes(bit_index) - 1], (bit_index % 8));

        } else {
            // Clear bit in the buffer.
            clear_8bit(&bit_buffer[bytes(bit_index) - 1], (bit_index % 8));
        }

        // Write the bytes in the buffer to the output file when the end of the buffer is reached.
        if (bit_index == BLOCK * 8) {
            int n = write_bytes(outfile, bit_buffer, BLOCK);
        }

        bit_index += 1;
    }

    for (int i = 0; i < 8; i += 1) {
        if (get_8bit(&sym, i) == 1) {
            // Set bit in the buffer.
            set_8bit(&bit_buffer[bytes(bit_index) - 1], (bit_index % 8));

        } else {
            // Clear bit in the buffer.
            clear_8bit(&bit_buffer[bytes(bit_index) - 1], (bit_index % 8));
        }

        // Write the bytes in the buffer to the output file when the end of the buffer is reached.
        if (bit_index == BLOCK * 8) {
            int b = write_bytes(outfile, bit_buffer, BLOCK);
        }

        bit_index += 1;
    }
}

//
// Inspired by Eugene's 3/5/2021 section.
// Write any remaining pairs of symbols and codes to the output file.
//
// outfile: The type int output file.
//
void flush_pairs(int outfile) {
    // Write remaining bits in the buffer.
    if (bit_index != 0) {
        write_bytes(outfile, bit_buffer, bytes(bit_index));
    }

    bit_index = 0; // Reset indexing in the buffer.
}

//
// Inspired by Eugene's 3/5/2021 section.
// Read a buffered pair of code and symbol from an input file into
// code and symbol pointers, respectively.
// Returns true if there are pairs left to read.
//
// infile: The type int input file.
// code: The pointer to store the code retrieved from the input file.
// sym: The pointer to store the symbol retrieved from the input file.
// bitlen: The number of bits to read for the code.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, int bitlen) {
    for (int i = 0; i < bitlen; i += 1) {
        // Read bytes into the buffer if the buffer is empty.
        if (bit_index == 0) {
            int b = read_bytes(infile, bit_buffer, BLOCK);
        }

        // Set bit from buffer into code.
        if (get_8bit(&bit_buffer[bytes(bit_index) - 1], (bit_index % 8)) == 1) {
            set_16bit(code, i);

        } else {
            clear_16bit(code, i);
        }

        bit_index += 1;

        // Reset buffer indexing when the end of buffer is reached.
        if (bit_index == BLOCK * 8) {
            bit_index = 0;
        }
    }

    for (int i = 0; i < 8; i += 1) {
        // Read bytes into the buffer if the buffer is empty.
        if (bit_index == 0) {
            int b = read_bytes(infile, bit_buffer, BLOCK);
        }

        // Set bit from buffer into symbol.
        if (get_8bit(&bit_buffer[bytes(bit_index) - 1], (bit_index % 8)) == 1) {
            set_8bit(sym, i);

        } else {
            clear_8bit(sym, i);
        }

        bit_index += 1;

        // Reset buffer indexing when the end of buffer is reached.
        if (bit_index == BLOCK * 8) {
            bit_index = 0;
        }
    }

    // There are no more pairs to be read if the code signals a STOP_CODE.
    return *code != STOP_CODE;
}

//
// Code inspired by Eugene's 3/5/2021 section.
// Write a pair to an output file. Each symbol is placed into a buffer
// and written out when it is filled.
//
// outfile: The type int output file.
// w: The word to write.
void write_word(int outfile, Word *w) {
    for (int i = 0; i < w->len; i += 1) {
        sym_buffer[sym_index] = w->syms[i + 2];
        sym_index += 1;

        if (sym_index == BLOCK * 8) {
            write_bytes(outfile, sym_buffer, BLOCK);
        }
    }
}

//
// Code inspired by Eugene's 3/5/2021 section.
// Write out any remaining symbols in the buffer to an output file.
//
void flush_words(int outfile) {
    // Write remaining symbols in the buffer.
    if (sym_index != 0) {
        write_bytes(outfile, sym_buffer, bytes(sym_index));
    }

    sym_index = 0; // Reset the indexing of the symbol buffer.
}
