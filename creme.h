#ifndef CREME_H
#define CREME_H

#include <sys/types.h>
#include <pthread.h>

/* Paramètres du Protocole*/
#define LPSEUDO 23
#define MAX_PSEUDO 64
#define BEUIP_PORT 9998        /* Port pour UDP (TP3) et TCP (TP1/2) */
#define BEUIP_MAGIC "BEUIP"
#define MAX_MSG 1024

/* Configuration Réseau et Système */
#define BROADCAST_IP "192.168.88.255" /* Consigne 7 : Défini une seule fois */
#define PUBLIC_DIR "pub"              /* Dossier pour les TP1/TP2 */

/* Structure de données (Liste Chaînée) */
struct elt {
    char nom[LPSEUDO + 1];
    char adip[16];
    struct elt *next;
};

/* Fonctions Cœur (creme.c)*/
int creme_beuip_start(const char *pseudo);
int creme_beuip_stop(void);
void creme_cleanup(void);
void creme_list(void);

/* Fonctions de gestion de la liste*/
void ajouteElt(char *pseudo, char *adip);
void supprimeElt(char *adip);

/* Fonctions de communication (Gescom / Biceps) */
int BeuipCmd(int n, char **p);
void beuip_ls_distant(char *pseudo);

#endif
