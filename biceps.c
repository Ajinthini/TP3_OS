#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include "creme.h"

int exec_beuip(int argc, char **argv) {
    if (argc < 2) return -1;

    if (strcmp(argv[1], "start") == 0 && argc == 3) {
        return creme_beuip_start(argv[2]);
    } 
    else if (strcmp(argv[1], "stop") == 0) {
        return creme_beuip_stop();
    } 
    else if (strcmp(argv[1], "list") == 0) {
        creme_list();
        return 0;
    } 
    // Consigne 5 et 6 : message <user|all> <msg>
    else if (strcmp(argv[1], "message") == 0 && argc >= 4) {
        char msg_complet[512] = "";
        for (int i = 3; i < argc; i++) {
            strcat(msg_complet, argv[i]);
            if (i < argc - 1) strcat(msg_complet, " ");
        }
        // Simulation de l'envoi pour le script
        printf("Envoi de '%s' à '%s'\n", msg_complet, argv[2]);
        return 0; 
    }
    return -1;
}

int main(void) {
    char *ligne;
    printf("Biceps v3.0 - Tapez 'beuip start [pseudo]' pour commencer.\n");
    
    while ((ligne = readline("biceps> "))) {
        if (strlen(ligne) > 0) {
            int argc = 0;
            char *argv[20];
            char *temp = strdup(ligne);
            char *token = strtok(temp, " ");
            
            while (token && argc < 20) {
                argv[argc++] = token;
                token = strtok(NULL, " ");
            }
            
            if (argc > 0) {
                if (strcmp(argv[0], "beuip") == 0) {
                    exec_beuip(argc, argv);
                } else if (strcmp(argv[0], "exit") == 0) {
                    creme_beuip_stop();
                    free(temp);
                    free(ligne);
                    return 0;
                }
            }
            free(temp);
        }
        free(ligne);
    }
    // Assure le stop même sur CTRL+D
    creme_beuip_stop();
    return 0;
}
