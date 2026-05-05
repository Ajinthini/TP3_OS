#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

/* Inclusion des modules des TP précédents pour la gestion des commandes (gescom) 
   et du cœur du protocole BEUIP (creme) */
#include "gescom.h"
#include "creme.h"

/* Fonction de sortie propre : libère la mémoire et arrête les services */
static int Sortie(int n, char **p) {
    (void)n;
    (void)p;
    
    /* Nettoyage du protocole creme (threads, listes) et des mots du shell */
    creme_cleanup(); 
    freeMots();
    
    printf("Arrêt du programme biceps\n");
    exit(0);
}

/* Gestionnaire de signal pour ignorer le comportement par défaut de CTRL+C */
static void handle_sigint(int sig) {
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
}

/* Enregistrement des commandes internes du shell développées aux TP1 et TP2 */
static void majComInt(void) {
    ajouteCom("exit", Sortie);
    ajouteCom("cd", ChangeDir);   /* Commande système TP1 */
    ajouteCom("pwd", PrintDir);  /* Commande système TP1 */
    ajouteCom("vers", Version);
    ajouteCom("beuip", BeuipCmd); /* Point d'entrée pour les commandes du TP3 */
}

/* Construction dynamique du prompt type shell (user@machine$) */
static char *build_prompt(void) {
    char hostname[256];
    char *user = getenv("USER");
    char symbol = (geteuid() == 0) ? '#' : '$'; /* Différenciation root/user */

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strcpy(hostname, "machine");
    }

    if (user == NULL) {
        user = "user";
    }

    size_t size = strlen(user) + strlen(hostname) + 4;
    char *prompt = malloc(size);
    if (prompt == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    snprintf(prompt, size, "%s@%s%c ", user, hostname, symbol);
    return prompt;
}

int main(void) {
    char buffer[1024];
    char *prompt_str;

    /* Configuration des signaux et initialisation des commandes */
    signal(SIGINT, handle_sigint);
    majComInt();

    /* Boucle principale de l'interpréteur de commandes */
    while (1) {
        /* Affichage du prompt utilisateur */
        prompt_str = build_prompt();
        printf("%s", prompt_str);
        fflush(stdout);

        /* Lecture de l'entrée utilisateur (gestion du CTRL+D) */
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            free(prompt_str);
            Sortie(0, NULL);
        }

        free(prompt_str);

        /* Suppression du caractère de nouvelle ligne */
        buffer[strcspn(buffer, "\n")] = '\0';

        /* Ignore les entrées vides */
        if (buffer[0] == '\0') {
            continue;
        }

        /* Analyse syntaxique de la commande (héritage TP1/TP2) */
        int count = analyseCom(buffer);
        
        /* Priorité aux commandes internes, sinon exécution comme commande système externe */
        if (count > 0 && !execComInt(count, Mots)) {
            execComExt(Mots);
        }
    }
    return 0;
}
