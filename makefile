# Makefile

# Compiler
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Nom de l'executable
TARGET = build/bibliotech.exe

# Dossier des fichiers source, objets et en-têtes
SRC_DIRS = src src/choix_users src/fonctions_principales src/fonctions_utilitaires src/MariaDB cron
OBJ_DIR = objects
INC_DIRS = include  # Ajoutez ici le dossier contenant vos fichiers d'en-tête

# Générer la liste des fichiers source
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Générer la liste des fichiers objets à partir des fichiers source
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Générer les options d'inclusion des fichiers d'en-tête
INC_FLAGS = $(foreach dir,$(INC_DIRS),-I$(dir))

# Règle par défaut
all:	$(TARGET)

# Règle générique pour la compilation des fichiers objets
$(OBJ_DIR)/%.o:	%.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

# Règle pour compiler l'exécutable
$(TARGET):	$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Règle de lancement

launch:	$(TARGET)
	./$(TARGET)

# Nettoyer les fichiers objets et l'exécutable
clean:	
	rm -rf $(OBJ_DIR)/*.o $(TARGET)