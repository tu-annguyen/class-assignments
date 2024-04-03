#include "bf.h"
#include "hash.h"
#include "parser.h"

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "mh:f:"
#define WORD    "[a-zA-Z0-9_'-]+"

int main(int argc, char **argv) {
    int opt;
    int ht_size = 10000;
    int bf_size = 1048576;
    bool mtf = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'm': mtf = true; break;
        case 'h':
            ht_size = atoi(optarg);
            if (ht_size <= 0) {
                fprintf(stderr, "-h only accepts positive integers.\n");
                return 1;
            }
            break;
        case 'f':
            bf_size = atoi(optarg);
            if (bf_size <= 0) {
                fprintf(stderr, "-f only accepts positive integers.\n");
                return 1;
            }
            break;
        default:
            fprintf(stderr, "Usage: %s -h <size> -f <size> -m\n", argv[0]);
            return 1;
            break;
        }
    }

    BloomFilter *bf = bf_create(bf_size);
    HashTable *ht = ht_create(ht_size, mtf);

    // Open badspeak.txt.
    FILE *bs_file = fopen("badspeak.txt", "r");
    if (bs_file == NULL) {
        fprintf(stderr, "badspeak.txt not found.\n");

        // Free allocated memory.
        bf_delete(&bf);
        ht_delete(&ht);

        return 1;
    }

    char badspeak[3000] = "";
    while (fscanf(bs_file, "%s\n", badspeak) != EOF) {
        bf_insert(bf, badspeak);
        ht_insert(ht, badspeak, NULL);
    }

    // Open newspeak.txt.
    FILE *ns_file = fopen("newspeak.txt", "r");
    if (bs_file == NULL) {
        fprintf(stderr, "newspeak.txt not found.\n");

        // Free allocated memory.
        bf_delete(&bf);
        ht_delete(&ht);

        return 1;
    }

    char oldspeak[3000] = "", newspeak[3000] = "";
    while (fscanf(ns_file, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }

    // Compile regex.
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");

        // Free allocated memory.
        clear_words();
        regfree(&re);

        return 1;
    }

    // Create list of badspeak and oldspeak words from stdin.
    char *word = NULL;
    LinkedList *bs_words = ll_create(mtf);
    LinkedList *os_words = ll_create(mtf);
    while ((word = next_word(stdin, &re)) != NULL) {
        if (bf_probe(bf, word)) {
            Node *n = ht_lookup(ht, word);

            if (n) {
                if (n->newspeak == NULL) {
                    ll_insert(bs_words, word, NULL);
                } else {
                    ll_insert(os_words, word, n->newspeak);
                }
            }
        }
    }

    // Message prompt to be outputted.
    if (ll_length(bs_words) != 0 && ll_length(os_words) != 0) {
        printf(
            "Dear Comrade,\n\nYou have chosen to use degenerate words that may cause hurtfeelings "
            "or cause your comrades to think unpleasant thoughts.This is doubleplus bad. To "
            "correct your wrongthink andpreserve community consensus we will be sending you to "
            "joycampadministered by Medellin’s Miniluv. Beware of the hippos.\n\nYour errors:\n\n");

        ll_print(bs_words);

        printf("\nThink of these words on your vacation!\n\n");

        ll_print(os_words);
    } else if (ll_length(bs_words) != 0 && ll_length(os_words) == 0) {
        printf(
            "Dear Comrade,\n\nYou have chosen to use degenerate words that may cause hurtfeelings "
            "or cause your comrades to think unpleasant thoughts.This is doubleplus bad. To "
            "correct your wrongthink andpreserve community consensus we will be sending you to "
            "joycampadministered by Medellin’s Miniluv. Beware of the hippos.\n\nYour errors:\n\n");

        ll_print(bs_words);
    } else if (ll_length(bs_words) == 0 && ll_length(os_words) != 0) {
        printf("Dear Comrade,\n\nSubmitting your text helps to preserve feelings and "
               "preventbadthink. Some of the words that you used are not goodspeak.The list shows "
               "how to turn the oldspeak words into newspeak.\n\n");

        ll_print(os_words);
    }

    // Free allocated memory.
    bf_delete(&bf);
    ht_delete(&ht);
    ll_delete(&bs_words);
    ll_delete(&os_words);
    clear_words();
    regfree(&re);
    fclose(bs_file);
    fclose(ns_file);

    return 0;
}
