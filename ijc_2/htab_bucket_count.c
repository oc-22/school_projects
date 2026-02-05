// htab_bucket_count.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program vrací velikost prvku v tabulce
#include "htab_struct.h"

size_t htab_bucket_count(const htab_t * t){
    return t->arr_size;
}
