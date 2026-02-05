// error.c
   // Řešení IJC-DU1, příklad b), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // Funkce pro vypisovani chyb a varovani
#include "error.h"
#include<stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void warning(const char *fmt,...){
    va_list args;
    va_start(args, fmt);
    fprintf(stderr,"Warning: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr,"\n");
    va_end(args);
    return;
}

void error_exit(const char *fmt,...){
    va_list args;
    va_start(args, fmt);
    fprintf(stderr,"Error: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr,"\n");
    va_end(args);
    exit(1);
    return;
}
