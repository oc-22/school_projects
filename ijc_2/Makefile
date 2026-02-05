# Makefile
# Řešení IJC-DU2, příklad b), 24.4.2025
# Autor: Ondrej Cagalinec, FIT
# Přeloženo: gcc Red Hat 14.2.1-7
# Make pro projekt

CC=gcc
CFLAGS= -Wall -Wextra -Werror -std=c11 -pedantic -O2

all: maxwordcount maxwordcount-dynamic tail

tail: tail.o
	$(CC) $^ $(CFLAGS) -o $@

maxwordcount: maxwordcount.o io.o libhtab.a
	$(CC) $(CFLAGS) -o $@ -static maxwordcount.o io.o -L. -lhtab

maxwordcount-dynamic: maxwordcount.o io.o libhtab.so
	$(CC) $(CFLAGS) -o $@ maxwordcount.o io.o -L. -lhtab

##################################################################################
tail.o: tail.c
	$(CC) $(CFLAGS) -c $< -o $@

maxwordcount.o: maxwordcount.c
	$(CC) $(CFLAGS) -c $< -o $@

########################################################################################

LIB_SOURCE=\
			htab_bucket_count.c \
			htab_clear.c \
			htab_erase.c \
			htab_find.c \
			htab_for_each.c \
			htab_free.c \
			htab_hash_function.c \
			htab_init.c \
			htab_lookup_add.c \
			htab_size.c

LIB_O=$(LIB_SOURCE:.c=.o)
LIB_O_DYN=$(LIB_SOURCE:.c=_dynamic.o)

%_dynamic.o: %.c htab.h
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

libhtab.a: $(LIB_O)
	ar rcs $@ $^

libhtab.so: $(LIB_O_DYN)
	$(CC) $(CFLAGS) -shared -o $@ $^

####################################################################################
run:
	./maxwordcount <tail.c
	LD_LIBRARY_PATH="." ./maxwordcount-dynamic <tail.c
clean:
	-rm -f *.o *.so *.a tail maxwordcount

zip:
	zip -r xcagalo00.zip *.c *.h Makefile
