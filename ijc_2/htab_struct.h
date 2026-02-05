// htab_struct.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Skryta struktura htab_item_t
#include "htab.h"
#include <stdlib.h>

typedef struct htab_item{
    htab_pair_t* pair;
    struct htab_item* next;
}htab_item_t;
struct htab{
    size_t size;
    size_t arr_size;
    htab_item_t** item;
};
