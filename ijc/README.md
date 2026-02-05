# IJC – DU1 Solution

Solution to the first homework assignment for the **IJC (C Programming)** course at FIT VUT.

## Overview
The solution implements a bitset data structure using an `unsigned long` array with support for both static and dynamic allocation. The bitset can be compiled either using macros or inline functions, selected via the `-DUSE_INLINE` compiler option.

The bitset is used to implement the **Sieve of Eratosthenes** for efficient prime number computation. Generated primes are further used to decode a hidden message stored in a binary PPM (P6) image using LSB steganography.

## Files
- `bitset.h` – bitset implementation  
- `eratosthenes.c` – Sieve of Eratosthenes  
- `primes.c` – prime number computation  
- `ppm.c/.h` – PPM image loading  
- `steg-decode.c` – steganographic message decoding  
- `error.c/.h` – error handling  
- `Makefile` – build rules

## Build
make
