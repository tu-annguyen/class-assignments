#include "bm.h"
#include "hamming.h"

#include <stdio.h>
#include <unistd.h>

#define OPTIONS "i:o:"

//
// Code from asgn4.pdf
// Packs two nibbles into a byte.
//
// upper: The upper nibble.
// lower: The lower nibble.
//
uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
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

    // Counting variables for program statistics.
    int num_bytes = 0, unc_errs = 0, c_errs = 0;
    double err_rate = 0.0;

    // Variables to store a byte from input.
    int16_t locode, hicode;

    while ((locode = fgetc(input)) != EOF
           && (hicode = fgetc(input)) != EOF) { // Read next two (or first two) characters of input.
        uint8_t lonibble, hinibble;
        ham_rc err;

        // Decode the lower nibble.
        if ((err = ham_decode(locode, &lonibble)) == HAM_ERR) {
            unc_errs += 1;
        } else if (err == HAM_ERR_OK) {
            c_errs += 1;
        }

        // Decode the upper nibble.
        if ((err = ham_decode(hicode, &hinibble)) == HAM_ERR) {
            unc_errs += 1;
        } else if (err == HAM_ERR_OK) {
            c_errs += 1;
        }

        uint8_t byte = pack_byte(hinibble, lonibble);

        num_bytes += 1;

        fputc(byte, output); // Write byte in output.
    }

    // Calculate error rate if there were uncorrectable errors.
    if (unc_errs != 0) {
        err_rate = (double) unc_errs / (double) num_bytes;
    }

    // Print program statistics.
    fprintf(stderr,
        "Total bytes processed: %d\nUncorrected errors: %d\nCorrected errors: %d\nError rate: %f\n",
        num_bytes, unc_errs, c_errs, err_rate);

    // Free allocated memory
    ham_destroy();
    fclose(input);
    fclose(output);

    return 0;
}
