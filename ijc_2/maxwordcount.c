// maxwordcount.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program nacte slovo z souboru
        //Prime number size is ideal for as little collisions as possible
        //https://stackoverflow.com/questions/3980117/hash-table-why-size-should-be-prime
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htab.h"
#include "io.h"

#define max_word_lenght 255

#define hash_size 17497

static size_t max_value = 0;

void find_max_value(htab_pair_t *pair) {
    if (pair->value > max_value) {
        max_value =pair->value;
    }
}

void print_max_words(htab_pair_t *pair) {
    if (pair->value == max_value) {
        printf("%s\t%u\n",pair->key,pair->value);
    }
}

int main(){

    htab_t* hash_table=htab_init(hash_size);
    char word[max_word_lenght+1];

    while (read_word(max_word_lenght,word, stdin)!=EOF) {

        htab_lookup_add(hash_table, word);
    }

    htab_for_each(hash_table,find_max_value);
    htab_for_each(hash_table, print_max_words);

    htab_clear(hash_table);
    htab_free(hash_table);

    return 0;
}
