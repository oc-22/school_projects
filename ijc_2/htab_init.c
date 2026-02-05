// htab_init.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program pro inicializaci tabulky
#include "htab_struct.h"
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

htab_t *htab_init(size_t n){
    if(n<=0){
        return NULL;
    }
    htab_t* hash_table=(htab_t*)malloc(sizeof(htab_t));
    if (hash_table==NULL) {
        return NULL;
    }
    hash_table->size=0;
    hash_table->arr_size=n;

    hash_table->item=(htab_item_t**)calloc(n,sizeof(htab_item_t*));
    if (hash_table->item==NULL) {
        free(hash_table);
        return NULL;
    }
    return hash_table;
}
