# SR01-Devoir-2

## Ex 1:
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

Cas 1 : Le premier fork() de l'expression ``` C( fork() || fork() )``` réussit

→ Un nouveau processus fils est créé.  De plus, dans le cas d'une exécution réussie, le deuxième fork() ne sera PAS exécuté, car c'est le comportement du OU logique ||). En effet, La particularité de || (OU) est qu'il s'arrête dès qu'une condition est vraie. Donc, dès qu'un fork() réussit, l'autre ne sera pas exécuté.
Donc, à ce stade, on  on a donc créée 2 processus fils : le premier fils créé dans la première partie de l'expression logique ET et le deuxième fils créé ici par le premier fork() de l'expression ``` C( fork() || fork() )``` .

Cas 2 : Le premier fork() de l'expression ``` C( fork() || fork() )```échoue

Le deuxième fork() sera alors exécuté, créant potentiellement un autre fils. Si c'est le cas alors on aura finalement créé 2 processus fils : le premier fils créé dans la première partie de l'expression logique ET et le deuxième fils créé ici par le deuxième fork() de l'expression ``` C( fork() || fork() )``` . Sinon, on n'a pas de deuxième processus fils créé, on n'a seulement 1 processus fils, celui est créé par le premier fork() avant le ET logique &&.

**Calcul du nombre  de processus créés**

Nombres possibles de processus créés : 

- 0 processus créé (le fork() de la première partie de l'expression logique ET échoue)
  
- 1 processus créé (le fork() de la première partie de l'expression logique ET réussit mais les 2 fork() de l'expression ```C ( fork() || fork() )``` échouent)
  
- 2 processus créés (le fork() de la première partie de l'expression logique ET réussit et au moins l'un des 2 fork() de l'expression ```C ( fork() || fork() )``` réussit)

  
