NOM: VAHEESAN
PRENOM: Ajinthini

# Projet Biceps v3.0 - Système de Messagerie Décentralisé

## Présentation
Ce projet implémente un système de communication en pair-à-pair (P2P) permettant la découverte d'utilisateurs sur un réseau local et l'échange de messages. Il a été réalisé dans le cadre du module de Système d'Exploitation (TP3).

## Architecture Logicielle
Le code est structuré de manière modulaire pour séparer l'interface utilisateur de la logique réseau :

### 1. Interpréteur de commandes (`biceps.c`)
Il constitue le point d'entrée du programme. 
- Utilise la bibliothèque `readline` pour une saisie interactive.
- Analyse les entrées utilisateur pour les transformer en appels de fonctions vers la bibliothèque `creme`.
- Gère proprement les signaux de sortie (`exit` ou `CTRL+D`) pour assurer la libération des ressources.

### 2. Bibliothèque Coeur (`creme.c` & `creme.h`)
C'est ici que réside la logique métier et réseau :
- **Gestion Multithread** : Utilisation de `pthread` pour faire tourner un serveur UDP en arrière-plan sans bloquer l'interpréteur de commandes.
- **Découverte Réseau** : Envoi et réception de paquets UDP sur le port 9998 en utilisant l'adresse de broadcast définie (`192.168.88.255`).
- **Structure de Données** : Les contacts sont stockés dans une liste chaînée dynamique, triée par pseudonyme pour une recherche et un affichage optimisés.
- **Synchronisation** : Utilisation de `pthread_mutex_t` pour protéger l'accès à la liste des contacts, évitant ainsi les conflits entre le thread réseau (qui écrit) et le thread principal (qui lit).

## Compilation et Tests
Le projet inclut un `Makefile` complet respectant les exigences de correction automatique :

* **`make`** : Compile le projet et génère l'exécutable `biceps`.
* **`make memory-leak`** : Génère le binaire `biceps-memory-leaks` avec les symboles de débogage (`-g`) et sans optimisations (`-O0`) pour une analyse précise via Valgrind.
* **`make clean`** : Nettoie tous les fichiers binaires et objets pour garantir un dépôt propre.

## Conformité Valgrind
Une attention particulière a été portée à la gestion de la mémoire. Lors de l'appel à `beuip stop`, l'ensemble de la liste chaînée est libéré (`free`) et les threads sont stoppés proprement, garantissant un "Error Summary: 0 errors" sous Valgrind.

## Auteur
* **NOM** : VAHEESAN
* **PRENOM** : Ajinthini
