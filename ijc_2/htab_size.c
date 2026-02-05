// htab_size.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // program vrací množství zapsaných prvků v tabulce
#include "htab_struct.h"

size_t htab_size(const htab_t * t){
    return t->size;
}
