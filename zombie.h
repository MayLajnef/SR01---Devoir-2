#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Définition des constantes
#define DURATION 60

// Prototypes des fonctions
void process_fils(void);
void process_pere(pid_t pid_fils);

#endif // ZOMBIE_H
