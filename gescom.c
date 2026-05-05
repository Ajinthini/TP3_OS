#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "gescom.h"

/* Variables globales pour stocker les arguments de la ligne de commande */
char *Mots[MAXMOTS];
int NMots = 0;

/* Tableau stockant les structures des commandes internes (nom + pointeur de fonction) */
static CommandInt tabComInt[NBMAXC];
static int nbComActual = 0;

/* Duplication d'une chaîne de caractères avec allocation dynamique sécurisée */
char *copyString(const char *s) {
    if (s == NULL) {
        return NULL;
    }

    char *copy = malloc(strlen(s) + 1);
    if (copy == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, s);
    return copy;
}

/* Libération de la mémoire allouée pour les mots de la commande précédente */
void freeMots(void) {
    for (int i = 0; i < NMots; i++) {
        free(Mots[i]);
        Mots[i] = NULL;
    }
    NMots = 0;
}

/* Analyse de la ligne de saisie pour extraire les mots (tokens) */
int analyseCom(char *b) {
    freeMots();

    char *token;
    /* Découpage selon les espaces, tabulations et retours à la ligne */
    while ((token = strsep(&b, " \t\n")) != NULL) {
        if (*token == '\0') {
            continue; /* Ignore les espaces multiples */
        }

        if (NMots >= MAXMOTS - 1) {
            fprintf(stderr, "Trop d'arguments\n");
            break;
        }

        Mots[NMots++] = copyString(token);
    }

    Mots[NMots] = NULL; /* Le tableau doit se terminer par NULL pour execvp */
    return NMots;
}

/* Enregistrement d'une nouvelle fonction dans le dictionnaire des commandes internes */
void ajouteCom(const char *nom, int (*f)(int, char **)) {
    if (nbComActual >= NBMAXC) {
        fprintf(stderr, "Trop de commandes internes\n");
        exit(EXIT_FAILURE);
    }

    tabComInt[nbComActual].nom = nom;
    tabComInt[nbComActual].fonction = f;
    nbComActual++;
}

/* Recherche et exécution d'une commande si elle appartient au tableau interne */
int execComInt(int n, char **p) {
    if (n == 0 || p[0] == NULL) {
        return 0;
    }

    for (int i = 0; i < nbComActual; i++) {
        if (strcmp(p[0], tabComInt[i].nom) == 0) {
            return tabComInt[i].fonction(n, p);
        }
    }

    return 0; /* Retourne 0 si la commande n'est pas interne */
}

/* Exécution d'une commande externe via la création d'un processus fils */
int execComExt(char **p) {
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        /* Processus fils : remplace son image par le programme externe */
        execvp(p[0], p);
        /* Si execvp revient, c'est qu'il y a eu une erreur */
        perror("execvp");
        _exit(EXIT_FAILURE);
    }

    /* Processus père : attend la fin de l'exécution du fils */
    waitpid(pid, NULL, 0);
    return 1;
}

/* Commande interne : Changement de répertoire (cd) */
int ChangeDir(int n, char **p) {
    const char *dest;

    if (n < 2) {
        /* Si aucun argument, direction le dossier HOME */
        dest = getenv("HOME");
        if (dest == NULL) {
            fprintf(stderr, "cd: HOME non défini\n");
            return 1;
        }
    } else {
        dest = p[1];
    }

    if (chdir(dest) != 0) {
        perror("cd");
    }

    return 1;
}

/* Commande interne : Affichage du répertoire courant (pwd) */
int PrintDir(int n, char **p) {
    (void)n;
    (void)p;

    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return 1;
    }

    printf("%s\n", cwd);
    return 1;
}

/* Commande interne : Affichage de la version du programme */
int Version(int n, char **p) {
    (void)n;
    (void)p;
    printf("biceps version 3.0\n");
    return 1;
}
