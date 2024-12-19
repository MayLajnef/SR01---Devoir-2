#include "exo2.h"

void ecrire_fils(int nb, char* name) {
    FILE* fichier = fopen(name, "w");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    fprintf(fichier, "%d", nb);
    printf("Entier %d écrit dans le fichier.\n", nb);

    fclose(fichier);
}

void lire_pere(int* nb, char* name) {
    FILE* fichier = fopen(name, "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    if (fscanf(fichier, "%d", nb) != 1) {
        perror("Erreur de lecture du fichier");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    printf("Entier lu: %d\n", *nb);
    fclose(fichier);

    // Suppression du fichier après lecture
    if (remove(name) != 0) {
        perror("Erreur de suppression du fichier");
    }
}

int entrer_nb() {
    int n, check_res;
    do {
        printf("veuillez entrer un nombre entier :  ");
        check_res = scanf("%d", &n);
        if (check_res != 1) {
            printf("Veuillez entrer un entier valide.\n");
            while (getchar() != '\n'); // Vider le buffer
        }
    } while (check_res != 1);
    return n;
}

void signal_handler(int sig) {
    if (sig == SIGUSR1) {
        printf("Signal reçu par le premier processus fils.\n");
    } else if (sig == SIGUSR2) {
        printf("Signal reçu par le processus père.\n");
    }
}

int main() {
    printf("Ici, le nombre est traité par le premier processus fils, ");
    int nb1 = entrer_nb();
    int nb2;
    do {
        printf("Ici, le nombre est traité par le deuxième processus fils, ");
        nb2 = entrer_nb();
    } while (nb2 == nb1);

    int nb1_lu, nb2_lu;

    int pid1 = fork();
    if (pid1 < 0) {
        perror("Erreur lors du fork du premier processus fils");
        return EXIT_FAILURE;
    }

    if (pid1 == 0) {
        // Premier processus fils
        signal(SIGUSR1, signal_handler);
        pause(); // Attente du signal

        ecrire_fils(nb1, "name1.txt");
        if (kill(getppid(), SIGUSR2) == -1) {
            perror("Erreur d'envoi de signal au père");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    int pid2 = fork();
    if (pid2 < 0) {
        perror("Erreur lors du fork du deuxième processus fils");
        return EXIT_FAILURE;
    }

    if (pid2 == 0) {
        // Deuxième processus fils
        ecrire_fils(nb2, "name2.txt");
        if (kill(pid1, SIGUSR1) == -1) {
            perror("Erreur d'envoi de signal au premier processus fils");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }

    // Processus père
    signal(SIGUSR2, signal_handler);
    pause(); // Attente du signal du premier fils

    lire_pere(&nb1_lu, "name1.txt");
    lire_pere(&nb2_lu, "name2.txt");

    for (int i = DURATION; i > 0; i--) {
        printf("Processus père encore vivant pour %d seconde(s)\n", i);
        sleep(1);
    }

    return 0;
}
