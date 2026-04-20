#ifndef CREME_H
#define CREME_H

#include <sys/types.h>
#include <pthread.h>

#define LPSEUDO 23
#define BROADCAST_IP "192.168.88.255" // Consigne 7 : Défini une seule fois

struct elt {
    char nom[LPSEUDO + 1];
    char adip[16];
    struct elt *next;
};

int creme_beuip_start(const char *pseudo);
int creme_beuip_stop(void);
void creme_list(void);

#endif
