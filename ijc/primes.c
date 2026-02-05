// primes.c
   // Řešení IJC-DU1, příklad a), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // Hledání posledních 10 prvočísel
#include "bitset.h"
#include "eratosthenes.h"
#include <stdio.h>
#include <time.h>


////////////////////////////////////////////////////////
//////////////////Recursion print Option////////////////
////////////////////////////////////////////////////////

// void rec_print_prime(unsigned long* p,char count, unsigned long index){
//     if (count == 0 || index ==(long unsigned)-1) {
//            return;
//        }
//        if (bitset_getbit(p, index) == 1) {
//            count--;
//            rec_print_prime(p, count, index - 1);
//            printf("%lu\n", index);
//        } else {
//            rec_print_prime(p, count, index - 1);
//        }
// }

int main(){

clock_t start = clock();
bitset_index_t n= 333000000;
int count = 0;
bitset_index_t index = n;
unsigned long primes[10];

bitset_create(p, 333000000);
Eratosthenes(p);

while(count!=10 && index>0){
    if (bitset_getbit(p, index)==1) {
        primes[count] = index;
        count++;
    }
    index--;
}
for (int idx = 9 ; idx>=0; idx--) {
    printf("%lu\n", primes[idx]);
}
//bitset_free(p);

fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
return 0;
}
