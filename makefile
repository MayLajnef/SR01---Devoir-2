# Variables
CC = gcc
CFLAGS = -Wall -Wextra -g
EXECS = exo2 exo3 zombie

# Règles
all: $(EXECS)

# Compilation individuelle
exo2: exo2.c
	$(CC) $(CFLAGS) -o exo2 exo2.c

exo3: exo3.c exo3.h
	$(CC) $(CFLAGS) -o exo3 exo3.c

zombie: zombie.c
	$(CC) $(CFLAGS) -o zombie zombie.c

# EXECUTION

# Exécution du code de zombie.c avec la commande make run_zombie
run_zombie: zombie
	@echo "Exécution du code de zombie.c (exercice 1) :"
	./zombie

# Exécution du code l'exercice 2 avec la commande make run_exo2
run_exo2: exo2
	@echo "Exécution de l'exercice 2 :"
	./exo2

# Exécution du code l'exercice 3 avec la commande make run_exo3
run_exo3: exo3
	@echo "Exécution de l'exercice 3 :"
	./exo3

# Nettoyage avec la commande make clean
clean:
	rm -f $(EXECS)
	rm -f *.o
