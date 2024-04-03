#include "code.h"
#include "endian.h"
#include "io.h"
#include "trie.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
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
            infile = open(optarg, O_RDONLY);

            if (infile == -1) {
                fprintf(stderr, "Failed to open input file.\n");
                return 1;
            }

            break;

        case 'o':
            outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600);

            if (outfile == -1) {
                fprintf(stderr, "Failed to open output file.\n");
                return 1;
            }

            break;
        }
    }

    FileHeader fh = { 0, 0 };

    // Get the file permissions in the form of a 16 bit unsigned integer.
    struct stat statbuf;
    fstat(outfile, &statbuf);
    uint16_t protection = statbuf.st_mode;

    fh.magic = MAGIC;
    fh.protection = protection;

    fchmod(outfile, fh.protection); // Set file permissions of outfile.

    write_header(outfile, &fh);

    if (big_endian()) { // Change header's magic and protection to little endian.
        swap32(fh.magic);
        swap16(fh.protection);
    }

    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;

    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;

    uint16_t next_code = START_CODE;

    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);

        if (next_node) {
            prev_node = curr_node;
            curr_node = next_node;

        } else {
            write_pair(outfile, curr_node->code, curr_sym, bit_length(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code);
            curr_node = root;
            next_code += 1;
        }

        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }

        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bit_length(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }

    write_pair(outfile, STOP_CODE, 0, bit_length(next_code));

    flush_pairs(outfile);

    // Free allocated memory.
    infile = close(infile);
    outfile = close(outfile);
    trie_delete(root);

    return 0;
}
