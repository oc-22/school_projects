// io.c
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Program nacte slovo z souboru
#include "io.h"

int read_word(unsigned max, char s[max], FILE *f){

    int c=fgetc(f);
    unsigned lenght=0;

    if (c==EOF) {
        return EOF;
    }
    if (isspace(c)) {
        while (isspace(c)) {
            c=fgetc(f);
        }
    }
    while (c!=EOF) {
        if ((isspace(c) && lenght!=0)) {
            s[lenght]='\0';
            return lenght;
        }
        s[lenght]=c;
        lenght++;
        if (lenght>max-1) {
            s[max]='\0';
            while((c=fgetc(f))!=EOF && !isspace(c) ){}
            return lenght;
        }
        c=fgetc(f);
    }
    s[lenght]='\0';
    return (lenght>0)?lenght:EOF;
}
