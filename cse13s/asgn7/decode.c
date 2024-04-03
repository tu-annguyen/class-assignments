#include "code.h"
#include "endian.h"
#include "io.h"
#include "trie.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "vi:o:"

//
// Return the number of bits needed to represent an unsigned 16 bit integer b.
//
int bit_length(uint16_t b) {
    int count = 1;
    uint16_t copy = b;

    while ((copy /= 2) > 0) {
        count += 1;
    }

    return count;
}

int main(int argc, char **argv) {
    // Variables for command-line options.
    int opt;
    int infile = STDIN_FILENO;
    int outfile = STDOUT_FILENO;
    bool stats = false;

    // Loop through all inputted command-line options.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': stats = true; break;

        case 'i':
            infile = open(optarg, O_RDONLY | O_CREAT);

            if (infile == -1) {
                fprintf(stderr, "Failed to open input file.\n");
                return 1;
            }

            break;
        }
    }

    FileHeader fh;

    read_header(infile, &fh);

    if (fh.magic == MAGIC) {
        if (big_endian()) { // Change header's magic and protection to little endian.
            swap32(fh.magic);
            swap16(fh.protection);
        }

    } else {
        fprintf(stderr, "Magic number does not match IO module's.\n");

        return 1;
    }

    WordTable *table = wt_create();

    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;

    while (read_pair(infile, &curr_code, &curr_sym, bit_length(next_code))) {
        table[next_code] = word_append_sym(table[curr_code], curr_sym);
        write_word(outfile, table[next_code]);
        next_code += 1;

        if (next_code == MAX_CODE) {
            wt_reset(table);
            next_code = START_CODE;
        }
    }

    flush_words(outfile);

    // Free allocated memory.
    infile = close(infile);
    outfile = close(outfile);
    wt_delete(table);

    return 0;
}
