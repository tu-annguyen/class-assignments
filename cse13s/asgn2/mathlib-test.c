#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "asctel"
#define HEADFMT "%2s %14s %20s %17s\n"
#define NUMFMT  "%7.4lf % 16.8lf % 16.8lf % 16.10lf\n"

//
// Prints the first two lines before each function calculation. This includes the header and dashes.
//
// funct: A string of the function-to-be-run's name
//
void print_header(char *funct) {
    printf(HEADFMT, "x", funct, "Library", "Difference");
    printf(HEADFMT, "-", "---", "-------", "----------");
}

//
// Inspired by Sahiti's section on 1/21/2021
//
int main(int argc, char **argv) {
    int opt;
    bool do_sin, do_cos, do_tan, do_exp, do_log = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            do_sin = true;
            do_cos = true;
            do_tan = true;
            do_exp = true;
            do_log = true;
            break;
        case 's': do_sin = true; break;
        case 'c': do_cos = true; break;
        case 't': do_tan = true; break;
        case 'e': do_exp = true; break;
        case 'l': do_log = true; break;
        default: fprintf(stderr, "Usage: %s -[asctel]\n", argv[0]); break;
        }
    }

    if (do_sin == true) {
        print_header("Sin");
        for (double x = -2 * M_PI; x < 2 * M_PI; x += 0.1) {
            printf(NUMFMT, x, Sin(x), sin(x), Sin(x) - sin(x));
        }
    }
    if (do_cos == true) {
        print_header("Cos");
        for (double x = -2 * M_PI; x < 2 * M_PI; x += 0.1) {
            printf(NUMFMT, x, Cos(x), cos(x), Cos(x) - cos(x));
        }
    }
    if (do_tan == true) {
        print_header("Tan");
        for (double x = M_PI / -3; x < M_PI / 3; x += 0.1) {
            printf(NUMFMT, x, Tan(x), tan(x), Tan(x) - tan(x));
        }
    }
    if (do_exp == true) {
        print_header("Exp");
        for (double x = 1; x < 10; x += 0.1) {
            printf(NUMFMT, x, Exp(x), exp(x), Exp(x) - exp(x));
        }
    }
    if (do_log == true) {
        print_header("Log");
        for (double x = 1; x < 10; x += 0.1) {
            printf(NUMFMT, x, Log(x), log(x), Log(x) - log(x));
        }
    }
}
