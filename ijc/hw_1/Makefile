# bitset.c
   # Řešení IJC-DU1, 24.3.2025
   # Autor: Ondrej Cagalinec, FIT
   # Soubor: Makefile
CC=gcc
CFLAGS= -std=c11 -Wall -Wextra -pedantic
OPTIMIZE= -O0

all: primes primes-i steg-decode
#######################################################################################################
#######################################################################################################
primes: primes.o eratosthenes.o error.o
	$(CC) $(CFLAGS) $^ -lm $(OPTIMIZE) -o $@

primes-i: primes.o eratosthenes.o bitset.o error.o
	$(CC) $(CFLAGS) $^ -lm $(OPTIMIZE) -DUSE_INLINE -o $@

steg-decode: steg-decode.o eratosthenes.o error.o bitset.o ppm.o
	$(CC) $(CFLAGS) $^ -lm $(OPTIMIZE) -o $@

#######################################################################################################
#######################################################################################################
primes.o: primes.c
	$(CC) $(OPTIMIZE) -c primes.c

eratosthenes.o: eratosthenes.c eratosthenes.h
	$(CC) $(OPTIMIZE) -c eratosthenes.c

bitset.o: bitset.c bitset.h
	$(CC) $(OPTIMIZE) -c bitset.c

error.o: error.c error.h
	$(CC) $(OPTIMIZE) -c error.c

steg-decode.o: steg-decode.c
	$(CC)  $(OPTIMIZE) -c steg-decode.c

ppm.o: ppm.c ppm.h
	$(CC)  $(OPTIMIZE) -c ppm.c
#######################################################################################################
#######################################################################################################

run: ./primes ./primes-i
	ulimit -s unlimited && ./primes && ./primes-i


#######################################################################################################
#######################################################################################################
clean:
	echo "Removing files..."
	rm primes primes-i steg-decode error.o eratosthenes.o primes.o bitset.o steg-decode.o ppm.o

#######################################################################################################
zip:
	zip xcagalo00.zip *.c *.h Makefile
