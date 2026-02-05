# IJC – DU1 Solution

Solution to the first homework assignment for the **IJC (C Programming)** course at FIT VUT.

## Overview
This project implements a **bitset** data structure using an `unsigned long` array with support for both static and dynamic allocation. The bitset is used in the **Sieve of Eratosthenes** for efficient prime number computation. The generated primes are also used to decode a hidden message stored in a binary PPM (P6) image via LSB steganography.

## Project Structure
- `bitset.c` / `bitset.h` – bitset implementation  
- `eratosthenes.c` / `eratosthenes.h` – Sieve of Eratosthenes  
- `primes.c` – program to compute primes  
- `ppm.c` / `ppm.h` – PPM image loading  
- `steg-decode.c` – decode hidden message from PPM  
- `error.c` / `error.h` – error handling utilities  
- `du1-obrazek.ppm` – test image for steganography  
- `Makefile` – build rules for all programs

## Build & Run
```bash
make        # compile all programs
make run    # compile and run
