CC = gcc
# Consigne 10 : Compilation stricte
CFLAGS = -Wall -Wextra -Werror -pthread
LDFLAGS = -lreadline -lpthread

# Consigne 1
all: biceps

biceps: biceps.o creme.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

biceps.o: biceps.c creme.h
	$(CC) $(CFLAGS) -c biceps.c

creme.o: creme.c creme.h
	$(CC) $(CFLAGS) -c creme.c

# Consigne 2 : Cible Valgrind
memory-leak: CFLAGS += -g -O0
memory-leak: clean
	$(CC) $(CFLAGS) -o biceps-memory-leaks biceps.c creme.c $(LDFLAGS)

# Consigne 0
clean:
	rm -f *.o biceps biceps-memory-leaks

.PHONY: clean all memory-leak
