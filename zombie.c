#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DURATION 60

int main() {
    pid_t pid_fils;

    pid_fils = fork();

    switch (pid_fils) {
        case -1: // fork() a échoué
            perror("fork échoué");
            exit(EXIT_FAILURE);

        case 0:  // Processus enfant
            printf("Fils : Mon PID est %d. Je me mets en sommeil pour %d secondes.\n", getpid(), DURATION);
            fflush(stdout);

            // 1. Se mettre en sommeil
            sleep(DURATION);

            // 2. Afficher un autre message après sommeil
            printf("Fils : Réveil terminé. Je termine mon processus.\n");
            fflush(stdout);

            // 3. Exécuter explicitement un autre appel système
            printf("Fils : Mon PID est toujours %d (appel système getpid).\n", getpid());

            // Terminer le processus enfant
            exit(EXIT_SUCCESS);

        default: // Processus père
            printf("Père : Mon fils a été créé avec PID = %d\n", pid_fils);

            // Le père attend que le fils termine
            wait(NULL);

            printf("Père : Le processus fils a terminé. Fin du processus père.\n");
    }

    return EXIT_SUCCESS;
}
