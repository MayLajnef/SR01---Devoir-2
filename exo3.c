#include "exo3.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SEUIL 10  // Seuil pour basculer vers une recherche séquentielle

/*
 * Fonction qui retourne le maximum de deux entiers.
 * @param i : premier entier
 * @param j : deuxième entier
 * @return Le maximum entre i et j
 */
int maxi(int i, int j) {
    // Comparaison des deux entiers et retour du plus grand
    return (i > j) ? i : j;
}

/*
 * Fonction qui retourne le maximum des éléments d'un tableau (les éléments sont compris entre un indice de début et un indice de fin).
 * @param tab : tableau d'entiers complet
 * @param debut : indice de début
 * @param fin : indice de fin
 * @return Le maximum des éléments du sous-tableau
 */
int max(int* tab, int debut, int fin) {
    int max_val = tab[debut]; // Initialisation avec le premier élément
    for (int i = debut + 1; i <= fin; i++) {
        max_val = maxi(tab[i], max_val); // Mise à jour du maximum trouvé
    }
    return max_val;
}

/*
 * Fonction pour écrire un entier dans un fichier
 * @param nb : entier à écrire
 * @param name : nom du fichier
 */
void ecrire_fils(int nb, char* name) {
    FILE* fichier = fopen(name, "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    fprintf(fichier, "%d", nb); // Écriture de l'entier dans le fichier
    fclose(fichier); // Fermeture du fichier
}

/*
 * Fonction pour lire un entier depuis un fichier et le supprimer
 * @param nb : pointeur où stocker l'entier lu
 * @param name : nom du fichier
 */
void lire_pere(int* nb, char* name) {
    FILE* fichier = fopen(name, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }
    if (fscanf(fichier, "%d", nb) != 1) {
        perror("Erreur de lecture du fichier");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }
    fclose(fichier); // Fermeture du fichier après lecture
    if (remove(name) != 0) {
        perror("Erreur lors de la suppression du fichier");
    }
}

/*
 * Fonction récursive pour trouver le maximum d'un tableau en parallèle.
 * @param tab : tableau d'entiers
 * @param debut : indice de début
 * @param fin : indice de fin
 * @return Le maximum du sous-tableau
 */
int max_parallele(int* tab, int debut, int fin) {
    // Cas de base : recherche séquentielle si le sous-tableau est petit
    if (fin - debut + 1 <= SEUIL) {
        return max(tab, debut, fin);
    }

    int milieu = (debut + fin) / 2; // Calcul de l'indice du milieu
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("Erreur lors du fork du premier fils");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        // Premier processus fils : recherche dans la moitié gauche
        int max_gauche = max_parallele(tab, debut, milieu);
        char filename[30];
        snprintf(filename, sizeof(filename), "fils1_%d.txt", getpid());
        ecrire_fils(max_gauche, filename);
        exit(EXIT_SUCCESS);
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("Erreur lors du fork du deuxième fils");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // Deuxième processus fils : recherche dans la moitié droite
        int max_droite = max_parallele(tab, milieu + 1, fin);
        char filename[20];
        snprintf(filename, sizeof(filename), "fils2_%d.txt", getpid());
        ecrire_fils(max_droite, filename);
        exit(EXIT_SUCCESS);
    }

    // Processus père : attendre les résultats des deux fils
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    int max_gauche, max_droite;
    char filename1[30], filename2[30];
    snprintf(filename1, sizeof(filename1), "fils1_%d.txt", pid1);
    snprintf(filename2, sizeof(filename2), "fils2_%d.txt", pid2);

    // Lire les résultats depuis les fichiers
    lire_pere(&max_gauche, filename1);
    lire_pere(&max_droite, filename2);

    // Retourner le maximum global
    return maxi(max_gauche, max_droite);
}

/*
 * Fonction principale pour tester la recherche parallèle du maximum dans un tableau.
 * Elle remplit un tableau avec des valeurs aléatoires et utilise max_parallele.
 */
int main() {
    int taille = 100;
    int* tableau = malloc(taille * sizeof(int));
    if (tableau == NULL) {
        perror("Erreur d'allocation mémoire");
        return EXIT_FAILURE;
    }

    // Initialisation du tableau avec des valeurs aléatoires
    srand(getpid());
    for (int i = 0; i < taille; i++) {
        tableau[i] = rand() % 1000;
    }

    // Recherche du maximum en parallèle
    int maximum = max_parallele(tableau, 0, taille - 1);

    printf("Le maximum trouvé est : %d\n", maximum);

    free(tableau); // Libération de la mémoire allouée
    return 0;
}

