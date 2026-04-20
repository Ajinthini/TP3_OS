#include "creme.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

static struct elt *liste_contacts = NULL;
static pthread_mutex_t mutex_liste = PTHREAD_MUTEX_INITIALIZER;
static int actif = 0;
static pthread_t tid_udp;
static char mon_pseudo[LPSEUDO + 1];

void ajouteElt(char *pseudo, char *adip) {
    pthread_mutex_lock(&mutex_liste);
    struct elt **curr = &liste_contacts;
    while (*curr && strcmp((*curr)->nom, pseudo) < 0) {
        curr = &((*curr)->next);
    }
    struct elt *nouveau = malloc(sizeof(struct elt));
    if (nouveau) {
        strncpy(nouveau->nom, pseudo, LPSEUDO);
        strncpy(nouveau->adip, adip, 15);
        nouveau->next = *curr;
        *curr = nouveau;
    }
    pthread_mutex_unlock(&mutex_liste);
}

void supprimeElt(char *adip) {
    pthread_mutex_lock(&mutex_liste);
    struct elt **curr = &liste_contacts;
    while (*curr) {
        if (strcmp((*curr)->adip, adip) == 0) {
            struct elt *temp = *curr;
            *curr = (*curr)->next;
            free(temp);
            break;
        }
        curr = &((*curr)->next);
    }
    pthread_mutex_unlock(&mutex_liste);
}

// Consigne 4 : Format strict pour le script automatique
void creme_list(void) {
    pthread_mutex_lock(&mutex_liste);
    struct elt *curr = liste_contacts;
    while (curr) {
        printf("%s : %s\n", curr->adip, curr->nom);
        curr = curr->next;
    }
    pthread_mutex_unlock(&mutex_liste);
}

void *serveur_udp(void *p) {
    (void)p;
    int sid = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in sin = {0};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9998);
    sin.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
    setsockopt(sid, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));
    bind(sid, (struct sockaddr *)&sin, sizeof(sin));

    while (actif) {
        char buf[512];
        struct sockaddr_in from;
        socklen_t len = sizeof(from);
        int n = recvfrom(sid, buf, 511, 0, (struct sockaddr *)&from, &len);
        if (n > 0) {
            buf[n] = '\0';
            if (buf[0] == '1') ajouteElt(buf + 6, inet_ntoa(from.sin_addr));
            else if (buf[0] == '0') supprimeElt(inet_ntoa(from.sin_addr));
        }
    }
    close(sid);
    return NULL;
}

int creme_beuip_start(const char *pseudo) {
    if (actif) return -1;
    actif = 1;
    strncpy(mon_pseudo, pseudo, LPSEUDO);
    pthread_create(&tid_udp, NULL, serveur_udp, NULL);
    return 0;
}

int creme_beuip_stop(void) {
    if (!actif) return -1;
    actif = 0;
    pthread_cancel(tid_udp);
    pthread_join(tid_udp, NULL);
    
    // Libération pour Valgrind
    pthread_mutex_lock(&mutex_liste);
    struct elt *curr = liste_contacts;
    while (curr) {
        struct elt *temp = curr;
        curr = curr->next;
        free(temp);
    }
    liste_contacts = NULL;
    pthread_mutex_unlock(&mutex_liste);
    return 0;
}
