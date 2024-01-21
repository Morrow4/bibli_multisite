# Makefile

# Compiler
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -g

# Nom de l'executable
TARGET = $(BUILD)/bibliotech.exe

# Dossier des fichiers source, objets, en-têtes et build
SRC_DIRS = src src/choix_users src/fonctions_principales src/fonctions_utilitaires src/MariaDB cron
OBJ_DIR = objects
BUILD = build
INC_DIRS = 	
MYSQL_INC = $(shell mysql_config --cflags)

# Générer la liste des fichiers source
SRCS = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Générer les options d'inclusion des fichiers d'en-tête
INC_FLAGS = $(foreach dir,$(INC_DIRS),-I$(dir)) $(MYSQL_INC)

# Générer la liste des fichiers objets à partir des fichiers source
OBJECTS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

# Règle par défaut
all: $(TARGET)

# Créer le dossier objects s'il n'existe pas
$(OBJ_DIR):
	@mkdir -p $@

# Règle générique pour la compilation des fichiers objets
$(OBJ_DIR)/%.o: $(OBJ_DIR) $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h))
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $(filter %$*.c,$(SRCS)) -o $@ 

# Créer le dossier build s'il n'existe pas
$(BUILD):
	@mkdir -p $@

# Règle pour compiler l'exécutable
$(TARGET): $(BUILD) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ -lmysqlclient

# Règle de lancement
launch: $(TARGET)
	./$(TARGET)

# Nettoyer les fichiers objets et l'exécutable
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)