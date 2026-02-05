// htab_for_each.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program prochází všechny prvky tabulky a aplikuje na každý z nich funkci f.
#include "htab_struct.h"
#include <stdio.h>

void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){
    for (size_t i=0;i<t->arr_size;i++) {
        if (t->item[i]!=NULL) {
            htab_item_t* current = t->item[i];
            while (current!=NULL) {
                htab_item_t* next = current->next;
                f(current->pair);
                current=next;
            }
        }
    }
}
