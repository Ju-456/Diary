# Nom de l'exécutable final
TARGET = main

# Options de compilation
CC = gcc
CFLAGS = -Wall -Wextra -g

# Liste des fichiers sources
SRCS = main.c MenuAndDeco.c page.c user.c

# Génère une liste des fichiers objets correspondants
OBJS = $(SRCS:.c=.o)

# Règle par défaut : compile le programme
all: $(TARGET)

# Règle pour générer l'exécutable en liant les objets
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Règle de compilation pour chaque fichier .c en un fichier .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés (.o et l'exécutable)
clean:
	rm -f $(OBJS) $(TARGET)

# Règle pour tout nettoyer, y compris les fichiers temporaires
veryclean: clean
	rm -f *~ *.bak

# Règle pour recompiler en nettoyant d'abord
rebuild: clean all

# Indique au Makefile que 'clean', 'veryclean' et 'rebuild' ne sont pas des fichiers
.PHONY: all clean veryclean rebuild
