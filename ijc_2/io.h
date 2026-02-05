// io.h
   // Řešení IJC-DU2, příklad b), 24.4.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 15.0.1-0
   // Hlavičkový soubor pro funkce pro čtení slov z souboru
#include <stdio.h>
#include <ctype.h>

int read_word(unsigned max, char s[max], FILE *f);
