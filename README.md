# SR01-Devoir-2

## Ex 1 :
On exécute la commande C suivante :
``` C
   fork() && ( fork() || fork() );
```

**Condition && (ET logique)**

Tout d'abord, on tente d'exécuter le fork() initial (première partie de l'expression logique ET).

Cas 1 : Le fork situé avant le ET logique && réussit son exécution (retourne une valeur non nulle pour le père)

→ À ce stade, le premier fork() a créé un premier processus fils ; on a donc 2 processus : le père et le premier fils. On passe alors à l'exécution de l'expression suivante.

Cas 2 : Le fork situé avant le ET logique && échoue

→ On ne crée aucun processus fils et l'expression suivante (située après &&)  ne sera pas exécutée, car c'est le comportement du ET logique. L'exécution de la commande C donnée s'arrête là. On aura donc créé 0 processus au final.

**Expression ( fork() || fork() )**

C'est un OU logique, ce qui signifie qu'un des deux fork() au plus sera exécuté. Ici, on tente d'exécuter le premier fork situé avant le OU logique ||.

Cas 1 : Le premier fork() de l'expression ``` ( fork() || fork() )``` réussit

→ Un nouveau processus fils est créé.  De plus, dans le cas d'une exécution réussie, le deuxième fork() ne sera PAS exécuté, car c'est le comportement du OU logique ||). En effet, La particularité de || (OU) est qu'il s'arrête dès qu'une condition est vraie. Ainsi, à ce stade, on a donc créée 2 processus fils : le premier fils créé dans la première partie de l'expression logique ET et le deuxième fils créé ici par le premier fork() de l'expression ``` ( fork() || fork() )``` .

Cas 2 : Le premier fork() de l'expression ```( fork() || fork() )```échoue

Le deuxième fork() sera alors exécuté, créant potentiellement un autre fils. Si c'est le cas alors on aura finalement créé 2 processus fils : le premier fils créé dans la première partie de l'expression logique ET et le deuxième fils créé ici par le deuxième fork() de l'expression ```( fork() || fork() )``` . Sinon, on n'a pas de deuxième processus fils créé, on n'a seulement 1 processus fils, celui est créé par le premier fork() avant le ET logique &&.

**Calcul du nombre  de processus créés**

Nombres possibles de processus créés : 

- 0 processus créé (le fork() de la première partie de l'expression logique ET échoue)
  
- 1 processus créé (le fork() de la première partie de l'expression logique ET réussit mais les 2 fork() de l'expression ```( fork() || fork() )``` échouent)
  
- 2 processus créés (le fork() de la première partie de l'expression logique ET réussit et au moins l'un des 2 fork() de l'expression ``` ( fork() || fork() )``` réussit)

Voici l'arbre généalogique des processus : 
``` perl
Père
 └── Fils1 (créé par le premier fork avant &&)
       ├── Fils2   (créé par le premier fork dans l'`||`)
       └── Fils2-bis (créé par le deuxième fork dans l'`||`, si le premier échoue)
```

## Ex 2 :

## Ex 3 :
1)
Voici le code de la fonction maxi() :
``` C
 /*
 * Fonction qui retourne le maximum de deux entiers.
 * @param i : premier entier
 * @param j : deuxième entier
 * @return Le maximum entre i et j
 */
int maxi(int i, int j) {
    return (i > j) ? i : j;
}
``` 

La fonction maxi() retourne le maximum entre 2 entiers. Pour cela, il nous aura suffit d'utiliser l'opérateur ternaire conditionnel ``` condition ? valeur_si_vrai : valeur_si_faux```.

2) Voici le code de la fonction max() :

``` C
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
```
  
La fonction max() retourne le maximum d'un sous-tableau (ensemble des éléments du tableau complet ```tab``` qui sont compris entre un indice de début et un indice de fin). Tout d'abord, on initialise le maximum au premier élément du sous-tableau. Puis, on fait un parcourt séquentiel des éléments du sous-tableau, où on applique la fonction max() (cf. question 1) à chaque paire d'éléments consécutifs du sous-tableau. Finalement, on renvoie la dernière valeur de notre variable  ```tab```, qui est le maximum parmi tous les éléments du sous tableau. 

3) 
Dans la fonction lire_pere(), le fichier dont le nom est donné en argument est détruit après lecture via remove().
La fonction max_parallele() implémente la logique de recherche parallèle du maximum :

Si le sous-tableau est petit (moins de SEUIL éléments), on fait une recherche séquentielle.
Sinon, on divise le tableau en deux et on crée deux processus fils.
Chaque fils cherche le maximum de sa moitié.
Les résultats sont écrits dans des fichiers temporaires.
Le père attend ses fils, lit leurs résultats et retourne le maximum.


La fonction main() crée un tableau de 100 éléments avec des valeurs aléatoires et appelle max_parallele() pour trouver le maximum.

  
