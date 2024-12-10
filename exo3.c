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
    int max = tab[debut];
    for (int i = debut + 1; i <= fin; i++) {
        max = maxi(tab[i], max);
    }
    return max;
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
        exit(1);
    }
    fprintf(fichier, "%d", nb);
    fclose(fichier);
}

/*
 * Fonction pour lire un entier depuis un fichier et le détruire
 * @param nb : pointeur où stocker l'entier lu
 * @param name : nom du fichier
 */
void lire_pere(int* nb, char* name) {
    FILE* fichier = fopen(name, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }
    fscanf(fichier, "%d", nb);
    fclose(fichier);
    remove(name);
}

/*
 * Fonction récursive pour trouver le maximum d'un tableau en parallèle
 * @param tab : tableau d'entiers
 * @param debut : indice de début
 * @param fin : indice de fin
 * @return Le maximum du sous-tableau
 */
int max_parallele(int* tab, int debut, int fin) {
    // Cas de base : si le sous-tableau est petit, recherche séquentielle
    if (fin - debut + 1 <= SEUIL) {
        return max(tab, debut, fin);
    }

    int milieu = (debut + fin) / 2;
    
    // Créer deux fils pour chercher le max dans chaque moitié
    pid_t pid1 = fork();
    if (pid1 == 0) {  // Premier fils
        int max_gauche = max_parallele(tab, debut, milieu);
        char filename1[20];
        snprintf(filename1, sizeof(filename1), "fils1_%d.txt", getpid());
        ecrire_fils(max_gauche, filename1);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {  // Deuxième fils
        int max_droite = max_parallele(tab, milieu + 1, fin);
        char filename2[20];
        snprintf(filename2, sizeof(filename2), "fils2_%d.txt", getpid());
        ecrire_fils(max_droite, filename2);
        exit(0);
    }

    // Processus père attend ses fils et récupère leurs résultats
    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    int max_gauche, max_droite;
    char filename1[20], filename2[20];
    snprintf(filename1, sizeof(filename1), "fils1_%d.txt", pid1);
    snprintf(filename2, sizeof(filename2), "fils2_%d.txt", pid2);

    lire_pere(&max_gauche, filename1);
    lire_pere(&max_droite, filename2);

    return maxi(max_gauche, max_droite);
}

int main() {
    // Exemple de tableau pour tester
    int taille = 100;
    int* tableau = malloc(taille * sizeof(int));

    // Remplissage du tableau avec des valeurs aléatoires
    srand(getpid());
    for (int i = 0; i < taille; i++) {
        tableau[i] = rand() % 1000;
    }

    // Trouver le maximum en parallèle
    int maximum = max_parallele(tableau, 0, taille - 1);

    printf("Le maximum trouvé est : %d\n", maximum);

    // Libérer la mémoire
    free(tableau);

    return 0;
}