# Makefile

# Compiler
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Nom de l'executable
TARGET = build\bibliotech.exe

# Dossier des fichiers source et objet
SRC_DIRS = src src\choix_users src\fonctions_principales src\fonctions_utilitaires src\MariaDB cron
OBJ_DIR = objects

# Générer la liste des fichiers source
SRC = $(foreach dir,$(SRC_DIRS),$(wildcard $(SRC_DIRS)/*.c))

# Générer la liste des fichiers objets à partir des fichiers source
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC)))

# Règle par défaut
all: $(TARGET)

# Règle pour compiler l'exécutable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle générique pour la compilation des fichiers objets
%.o: $(SRC_DIRS)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Règle de lancement

launch: ./$(TARGET)

# Nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJECTS) $(TARGET)