#ifndef EXO2_H
#define EXO2_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include <signal.h>
#define DURATION 5

void ecrire_fils(int nb, char* name);
void lire_pere(int* nb, char* name);
int entrer_nombre();
void signal_handler(int sig);

#endif
