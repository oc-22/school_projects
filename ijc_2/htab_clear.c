// htab_clear.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program vymaže všechny prvky v tabulce
#include "htab_struct.h"
#include <stdlib.h>

void htab_clear(htab_t * t){
    if (t==NULL) {
        return;
    }
    for (size_t i=0;i<t->arr_size;i++) {
        if (t->item[i]!=NULL) {
            htab_item_t* current = t->item[i];
            while (current!=NULL) {
                htab_item_t* next = current->next;
                free((char*)current->pair->key);
                free(current->pair);

                t->item[i]=current->next;

                free(current);
                current=next;
            }
            t->item[i]=NULL;
        }
    }
}
