// htab_erase.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program odstraní prvek z tabulky
#include "htab.h"
#include "htab_struct.h"


bool htab_erase(htab_t * t, htab_key_t key){
    if (t == NULL || key == NULL){
        return false;
    }

    size_t idx=htab_hash_function(key)%t->arr_size;

    htab_item_t* current=t->item[idx];
    htab_item_t* prev=NULL;

    while (current!=NULL) {
        htab_item_t* next = current->next;
        if (strcmp(current->pair->key,key)==0) {
            t->size--;
            if (current->pair->key!=NULL) {
                free((char*)current->pair->key);
                current->pair->key=NULL;
            }
            if (current->pair!=NULL) {
                free(current->pair);
                current->pair=NULL;
            }
            if (prev==NULL) {
                t->item[idx]=current->next;
                free(current);
                current=NULL;
                return true;
            }
            else{
                if(current->next!=NULL){
                    prev->next=current->next;
                }
                free(current);
                current=NULL;
                return true;
            }
        }
    prev=current;
    current=next;
    }
    return false;
}
