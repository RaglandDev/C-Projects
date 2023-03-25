#include "word.h"
#include "code.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Creates a new Word with symbols syms and length len
 * Allocates new array and copies the symbols over
 */
Word *word_create(uint8_t *syms, uint32_t len) {
    Word *word = (Word *) calloc(1, sizeof(Word));
    assert(word != NULL);

    uint8_t *symbols = (uint8_t *) calloc(1, sizeof(uint8_t) * len);
    assert(symbols != NULL);

    // Copy symbols into new array
    for (uint32_t i = 0; i < len; i++) {
        symbols[i] = syms[i];
    }

    // Set members
    word->syms = symbols;
    word->len = len;

    return word;
}

/*
 * Creates a new word by appending symbol sym to word w
 * Updates the length of the new word and copies symbols over
 * Returns a pointer to the newly allocated word
 */
Word *word_append_sym(Word *w, uint8_t sym) {
    uint32_t new_length = w->len + 1;

    Word *new_word = (Word *) calloc(1, sizeof(Word));
    assert(new_word != NULL);

    uint8_t *new_symbols = calloc(new_length, sizeof(uint8_t));
    assert(new_symbols != NULL);

    // Copy old symbols into array
    for (uint32_t i = 0; i < w->len; i++) {
        new_symbols[i] = w->syms[i];
    }

    // Add extra symbol
    new_symbols[new_length - 1] = sym;

    // Set members
    new_word->syms = new_symbols;
    new_word->len = new_length;

    return new_word;
}

/*
 * Deletes the word w
 * Frees up associated space
 */
void word_delete(Word *w) {
    if (w != NULL) {
        free(w->syms);
        free(w);
        w = NULL;
    }
}

/*
 * Constructor:
 * Creates a new table big enough to fit MAX_CODE
 * Creates the first element at EMPTY_CODE and returns it
 */
WordTable *wt_create(void) {
    WordTable *word_table = calloc(MAX_CODE, sizeof(Word *));
    assert(word_table != NULL);

    // Initialize first element at EMPTY_CODE
    word_table[EMPTY_CODE] = word_create(NULL, 0);

    return word_table;
}

/*
 * Deletes all words except EMPTY_CODE
 * Frees associated memory
 */
void wt_reset(WordTable *wt) {
    for (int i = 0; i < MAX_CODE; i++) {
        if (i != EMPTY_CODE && wt[i] != NULL) {
            word_delete(wt[i]);
            wt[i] = NULL;
        }
    }
}

/*
 * Destructor: Deletes all words and tables
 * Frees up associated memory
 */
void wt_delete(WordTable *wt) {
    for (int i = 0; i < MAX_CODE; i++) {
        word_delete(wt[i]);
    }
    free(wt);
}
