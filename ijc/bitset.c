// bitset.c
   // Řešení IJC-DU1, příklad a), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // Implementace inline funkcí pro bitset.h
#include "bitset.h"

#ifdef USE_INLINE
    extern void bitset_free(unsigned long *jmeno_pole);
    extern unsigned long bitset_size(unsigned long *jmeno_pole);
    extern void bitset_fill(unsigned long *jmeno_pole, unsigned bool_vyraz);
    extern void bitset_setbit(unsigned long *jmeno_pole, unsigned long index, unsigned vyraz);
    extern unsigned bitset_getbit(unsigned long *jmeno_pole,unsigned long index);

#else

#endif
