// ppm.c
   // Řešení IJC-DU1, příklad b), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // hlavičkový soubor pro ppm.c
#ifndef ppm_h
#define ppm_h

#include <stdio.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ppm{
    unsigned xsize;
    unsigned ysize;
    char data[];
};

struct ppm * ppm_read(const char * filename);
void ppm_free(struct ppm *p);

#endif
