#include "bm.h"
#include "hamming.h"

#include <stdio.h>
#include <unistd.h>

#define OPTIONS "i:o:"

//
// Code from asgn4.pdf.
// Returns the lower nibble of val.
//
uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

//
// Code from asgn4.pdf.
// Returns the upper nibble of val.
//
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

int main(int argc, char **argv) {
    // Variables for command-line options.
    int opt;
    FILE *input = stdin, *output = stdout;

    // Loop though all inputted command-line options.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            input = fopen(optarg, "r");
            if (input == NULL) {
                fprintf(stderr, "File provided for input is NULL.\n");
                fclose(input); // Free memory allocated for input
                return 0;
            }
            break;
        case 'o': output = fopen(optarg, "w"); break;
        default:
            fprintf(stderr, "Usage: %s -[i:o:]\n", argv[0]);
            return 0;
            break;
        }
    }

    if (ham_init() != HAM_OK) { // Initialize generator and parity bit matricies.
        fprintf(stderr, "Failed to initialize hamming module.\n");
        return 0;
    }

    int16_t byte;

    while ((byte = fgetc(input)) != EOF) { // Read next (or first) character of input.
        uint8_t lonibble = lower_nibble(byte);
        uint8_t hinibble = upper_nibble(byte);

        uint8_t locode, hicode;
        ham_rc err;

        if ((err = ham_encode(lonibble, &locode)) == HAM_ERR) {

            fprintf(stderr, "Failed to encode lower nibble.\n");
            return 0;
        }

        if ((err = ham_encode(hinibble, &hicode)) == HAM_ERR) {
            fprintf(stderr, "Failed to encode upper nibble.\n");
            return 0;
        }

        // Write locode and hicode to output.
        fputc(locode, output);
        fputc(hicode, output);
    }

    // Free allocated memory
    ham_destroy();
    fclose(input);
    fclose(output);

    return 0;
}
