// error.c
   // Řešení IJC-DU1, příklad b), 24.3.2025
   // Autor: Ondrej Cagalinec, FIT
   // Přeloženo: gcc Red Hat 14.2.1-7
   // rozhraní pro error.c
#ifndef error_h
#define error_h

void warning(const char *fmt, ...);
void error_exit(const char *fmt,...);

#endif
