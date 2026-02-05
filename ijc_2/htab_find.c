// htab_find.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: Red Hat 15.0.1-0
   // Program vyhledá prvek v tabulce
#include "htab.h"
#include "htab_struct.h"
#include <stddef.h>
#include <stdlib.h>


htab_pair_t * htab_find(const htab_t * t, htab_key_t key){

    size_t idx=htab_hash_function(key)%t->arr_size;

    htab_item_t* current=t->item[idx];

    while(current!=NULL){
        htab_item_t* next=current->next;
        if (strcmp(current->pair->key, key)==0) {
            return current->pair;
        }
        current=next;
        }

    return NULL;
}
