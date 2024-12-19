#ifndef EXO3_H
#define EXO3_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SEUIL 10  // Seuil pour basculer vers une recherche séquentielle

int maxi(int i, int j);
int max(int* tab, int debut, int fin);
void ecrire_fils(int nb, char* name);
void lire_pere(int* nb, char* name);
int max_parallele(int* tab, int debut, int fin);

#endif
