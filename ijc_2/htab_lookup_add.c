// htab_lookup_add.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program pro přidání prvku do tabulky
#include "htab.h"
#include "htab_struct.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key){

    if (t==NULL || key==NULL) {
        return NULL;
    }
    htab_pair_t* find=htab_find(t,key);
    if (find!=NULL) {
        find->value++;
        return find;
    }

    size_t idx= htab_hash_function(key)%t->arr_size;
    htab_item_t* new_item=malloc(sizeof(htab_item_t));

    if (new_item==NULL) {
        return NULL;
    }
    new_item->pair=malloc(sizeof(htab_pair_t));
    if (new_item->pair==NULL) {
        free(new_item);
        return NULL;
    }
    new_item->pair->key=malloc(strlen(key)+1);
    if (new_item->pair->key==NULL) {
        free(new_item->pair);
        free(new_item);
        return NULL;
    }
    strcpy((char*)new_item->pair->key,key);
    new_item->pair->value=0;

    new_item->next=t->item[idx];
    t->item[idx]=new_item;
    new_item->pair->value++;
    t->size++;

    return new_item->pair;
}
