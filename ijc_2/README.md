# IJC – DU2 Solution

Solution to the second homework assignment (DU2) for the **IJC (C Programming)** course at FIT VUT.

## Overview

This project implements two main programs:

1. **Tail program (`tail.c`)**  
   - Prints the last N lines of a given file (default 10).  
   - Supports reading from stdin if no file is provided.  
   - Supports `-n` option for custom number of lines.  
   - Limits line length (e.g., 511 chars) and warns on overflow.

2. **Word count program (`maxwordcount.c` / `maxwordcount-dynamic`)**  
   - Counts word occurrences from input using a **custom hash table library**.  
   - `maxwordcount` uses the **static library**, `maxwordcount-dynamic` uses the **shared library**.  
   - Efficient hash table implementation with functions: init, find, add, erase, clear, free, size, bucket_count, for_each.  
   - Memory management is dynamic; long words are truncated with a warning.  
   - Uses `io.c` for word reading.

## Project Structure

- `tail.c` – tail implementation  
- `maxwordcount.c` – word counting with static library  
- `io.c` / `io.h` – word reading functions  
- `htab.h` – hash table interface  
- `htab_struct.h` – internal hash table structure  
- `htab_*.c` – hash table function implementations: init, find, add, erase, clear, free, size, bucket_count, for_each, hash_function  
- `Makefile` – build all programs and libraries  

## Build & Run

```bash
make                 # build all programs and libraries
make run             # build and run
