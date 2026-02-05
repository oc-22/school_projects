// bitset.h
   // Řešení IJC-DU1, příklad a), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // Hlavičkový soubor pro bitset.c
#ifndef bitset_h
#define bitset_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "error.h"

typedef unsigned long *bitset_t;
typedef unsigned long bitset_index_t;

#define BIT_SIZE_OF_LU (CHAR_BIT*sizeof(unsigned long))

#define UL_INDEX(index) ((index/BIT_SIZE_OF_LU)+1UL)

#define INDEX_POSITION(index) (index%BIT_SIZE_OF_LU)

#define NUM_OF_LU_IN_BITSET(velikost) ((velikost/BIT_SIZE_OF_LU+((velikost%BIT_SIZE_OF_LU>0)?1:0))+1UL)

#define bitset_create(jmeno_pole, velikost) \
    static_assert(velikost>0,"Velikost musi byt kladne cislo!");\
    unsigned long jmeno_pole[NUM_OF_LU_IN_BITSET(velikost)];\
    for(bitset_index_t idx = 1; idx<NUM_OF_LU_IN_BITSET(velikost); idx++){\
        jmeno_pole[idx] = 0;\
    }\
    jmeno_pole[0] = velikost;


#define bitset_alloc(jmeno_pole, velikost)\
    assert(velikost>0);\
    bitset_t jmeno_pole = (unsigned long*)calloc(NUM_OF_LU_IN_BITSET(velikost), sizeof(unsigned long));\
    if(jmeno_pole==NULL){\
        error_exit("bitset_alloc: Chyba alokace pameti");\
    }\
    jmeno_pole[0] = velikost;\


#ifdef USE_INLINE
    inline void bitset_free(unsigned long *jmeno_pole){
        free(jmeno_pole);
    }
     inline unsigned long bitset_size(unsigned long *jmeno_pole){
        return jmeno_pole[0];
    }
     inline void bitset_fill(unsigned long *jmeno_pole, unsigned bool_vyraz){
        unsigned size = NUM_OF_LU_IN_BITSET(jmeno_pole[0]);

        for(unsigned idx = 1;idx<size ;idx++){
            jmeno_pole[idx] = (bool_vyraz)? (~0UL):(0UL);
        }
    }
     inline void bitset_setbit(unsigned long *jmeno_pole, unsigned long index, unsigned vyraz){
        if(index>bitset_size(jmeno_pole)){
            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",
                           (unsigned long)index, (unsigned long)bitset_size(jmeno_pole));
        }
        if(vyraz==1){
            jmeno_pole[UL_INDEX(index)] |= (1UL<<INDEX_POSITION(index));
        }
        else{
            jmeno_pole[UL_INDEX(index)] &= ~(1UL<<INDEX_POSITION(index));
        }
    }
     inline unsigned bitset_getbit(unsigned long *jmeno_pole, unsigned long index){
        if(index>bitset_size(jmeno_pole)){

            error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",
                           (unsigned long)index, (unsigned long)bitset_size(jmeno_pole));
        }
        return (jmeno_pole[UL_INDEX(index)]>>INDEX_POSITION(index)& 1UL);
    }


#else

    #define bitset_free(jmeno_pole) free(jmeno_pole);

    #define bitset_size(jmeno_pole) jmeno_pole[0]

    #define bitset_fill(jmeno_pole,bool_vyraz)\
        do{\
            for(bitset_index_t idx = 1; idx<NUM_OF_LU_IN_BITSET(jmeno_pole[0]);idx++){\
                jmeno_pole[idx] = (bool_vyraz) ? ~0UL : 0UL;\
            };\
        }while(0);

    #define bitset_setbit(jmeno_pole,index,vyraz)\
        do{\
            if(index>=bitset_size(jmeno_pole)+1UL){\
                error_exit("bitset_setbit: Index %lu mimo rozsah 0..%lu",\
                               (unsigned long)index, (unsigned long)bitset_size(jmeno_pole));\
            }\
            if(vyraz==1){\
                jmeno_pole[UL_INDEX(index)] |=(1UL<<INDEX_POSITION(index));\
            }\
            else{\
                jmeno_pole[UL_INDEX(index)] &=~(1UL<<INDEX_POSITION(index));\
            }\
        }while(0);

    #define bitset_getbit(jmeno_pole,index)\
        (index>=bitset_size(jmeno_pole)+1UL) ? (error_exit("bitset_getbit: Index %lu mimo rozsah 0..%lu",\
            (unsigned long)index, (unsigned long)bitset_size(jmeno_pole)),1) : (jmeno_pole[UL_INDEX(index)] >> INDEX_POSITION(index) & 1UL)

#endif

#endif
