// htab_free.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program uvolní všechny prvky v tabulce
#include "htab.h"
#include "htab_struct.h"
#include <stdlib.h>

void htab_free(htab_t *t){
    if (t==NULL) {
        return;
    }
    for (size_t i=0; i<t->arr_size;i++) {
        htab_item_t* current=t->item[i];
        while (current!=NULL) {
            htab_item_t* next=current->next;
            if(current->pair->key!=NULL){
                free((char*)current->pair->key);
            }
            if (current->pair!=NULL) {
                free((htab_pair_t*)current->pair);
            }
            current=next;
        }
    }
    free(t->item);
    free(t);
}
