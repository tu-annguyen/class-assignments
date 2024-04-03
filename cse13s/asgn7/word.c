#include "word.h"

#include "code.h"

#include <stdlib.h>

//
// Create a Word and initialize its symbol array and length value.
//
// syms: An array to hold the word's symbols.
// len: The number of symbols in the symbol array.
//
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *w = malloc(sizeof(Word));

    if (w) { // If memory for the word was successfully allocated.
        w->syms = syms;
        w->len = len;

    } else { // Free the allocated memory, otherwise.
        free(w);
        w = NULL;
    }

    return w;
}

//
// Append a symbol at the end of a Word's symbol array. Return the new Word.
//
// w: The Word in question.
// sym: The symbol to append.
//
Word *word_append_sym(Word *w, uint8_t sym) {
    Word *x;

    if (!w || w->len == 0) { // Check if word does not exist or is empty.
        x = word_create(&sym, 1);

    } else {
        x = word_create(w->syms, w->len + 1);
        x->syms[w->len] = sym;

        word_delete(w);
    }

    return x;
}

//
// Free memory allocated for a Word.
//
// w: The Word to free.
//
void word_delete(Word *w) {
    free(w);
    w = NULL;
}

//
// Create a new WordTable with a pre-defined size of MAX_CODE.
// A single empty word is initialized in the WordTable.
//
WordTable *wt_create(void) {
    uint8_t syms;

    WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(Word *));

    wt[EMPTY_CODE] = word_create(&syms, 0);

    return wt;
}

//
// Reset a WordTable to only contain an empty word.
//
void wt_reset(WordTable *wt) {
    for (int i = 2; i < MAX_CODE; i += 1) {
        if (wt[i]) { // Check if word exists.
            word_delete(wt[i]);
        }
    }
}

//
// Frees allocated memory for a WordTable.
// Reset the WordTable and delete the empty word.
//
void wt_delete(WordTable *wt) {
    wt_reset(wt);

    word_delete(wt[0]);

    free(wt);
    wt = NULL;
}
