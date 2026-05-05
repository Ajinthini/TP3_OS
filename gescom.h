#ifndef GESCOM_H
#define GESCOM_H

#define MAXMOTS 100
#define NBMAXC 20

/* Structure pour associer un nom de commande à une fonction */
typedef struct {
    const char *nom;
    int (*fonction)(int, char **);
} CommandInt;

/* Variables globales pour l'analyse des commandes */
extern char *Mots[MAXMOTS];
extern int NMots;

/* Prototypes des fonctions de gestion du shell (Héritage TP1/TP2) */
int analyseCom(char *b);
void ajouteCom(const char *nom, int (*f)(int, char **));
int execComInt(int n, char **p);
int execComExt(char **p);
void freeMots(void);

/* Commandes internes obligatoires */
int ChangeDir(int n, char **p);
int PrintDir(int n, char **p);
int Version(int n, char **p);

#endif
