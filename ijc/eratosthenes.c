// eratosthenes.c
   // Řešení IJC-DU1, příklad a), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
    // Implementace funkce Eratosthenes pro bitset.h
#include "bitset.h"
#include "eratosthenes.h"
#include <math.h>

void Eratosthenes(bitset_t p){

    bitset_index_t mez = bitset_size(p);

    bitset_fill(p, 1);
    bitset_setbit(p, 1, 0);
    bitset_setbit(p, 0, 0);
    bitset_index_t i = 2;
    bitset_index_t j;
    while (i<=sqrt(mez)) {
        if((bitset_getbit(p, i))==1){
            j = i+i;
            while(j<=mez){
                bitset_setbit(p, j, 0);
                j+=i;
            }
        }
        i++;
    }
}
