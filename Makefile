#Définition du compilateur
CC = gcc

#Options de compilation standards (Consignes 1 et 10)
# -g : ajoute les symboles de débogage
# -pthread pour la gestion des threads (Consigne 8)
CFLAGS = -Wall -Wextra -Werror -g -pthread

#test de fuites mémoire Valgrind (Consigne 2)
MLFLAGS = -Wall -Wextra -Werror -g -O0 -pthread

all: biceps

# Compilation de l'exécutable principal (Consigne 1)
biceps: biceps.c gescom.c creme.c gescom.h creme.h
	$(CC) $(CFLAGS) -o biceps biceps.c gescom.c creme.c

# Cible pour préparer le test Valgrind
memory-leak: biceps-memory-leaks

# Compilation pour l'analyse mémoire (Consigne 2)
biceps-memory-leaks: biceps.c gescom.c creme.c gescom.h creme.h
	$(CC) $(MLFLAGS) -o biceps-memory-leaks biceps.c gescom.c creme.c

# Nettoyage des fichiers binaires et objets (Consigne 0)
clean:
	rm -f biceps biceps-memory-leaks *.o
