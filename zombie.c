
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#define DURATION 60

int main() {
    pid_t pid_fils;

    pid_fils = fork();

    switch (pid_fils) {
        case -1: // fork() a échoué
            perror("fork échoué");
            exit(EXIT_FAILURE);
            break;

        case 0:  // Processus fils
            for (int i = DURATION; i > 0; i--) {
                printf("Fils : je suis vivant pour encore %d secondes.\n", i);
                fflush(stdout);
                sleep(1);
            }
            printf("Fils : je termine maintenant.\n");
            exit(EXIT_SUCCESS);
            break;

        default: // Processus père
            printf("Père : mon processus fils a été créé avec PID = %d\n", pid_fils);
            sleep(120); // Le père reste en vie pour observer le zombie
            printf("Père : fin du processus.\n");
            break;
    }

    return EXIT_SUCCESS;
}
