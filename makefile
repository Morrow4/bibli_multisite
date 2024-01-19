# Makefile

# Compiler
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Nom de l'executable
TARGET = build/bibliotech.exe

# Dossier des fichiers source et objet
SRC_DIRS = src src/choix_users src/fonctions_principales src/fonctions_utilitaires src/MariaDB cron
OBJ_DIR = objects

# Générer la liste des fichiers source
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Générer la liste des fichiers objets à partir des fichiers source
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Règle par défaut
all:	$(TARGET)

# Règle générique pour la compilation des fichiers objets
$(OBJECTS):	$(SRCS)
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour compiler l'exécutable
$(TARGET):	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Règle de lancement

launch:	
	./$(TARGET)

# Nettoyer les fichiers objets et l'exécutable
clean:	
	rm -rf $(OBJ_DIR)/*.o $(TARGET)