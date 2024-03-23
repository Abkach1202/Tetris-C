# Gestion des repertoires
SRC_DIR ?= src
BUILD_DIR ?= build
OBJ_DIR ?= obj

# Gestion du compilateur et des options
CC ?= gcc
CFLAGS ?= -Wall -MMD -g
LDFLAGS = $(shell sdl2-config --cflags --libs) -lSDL2_ttf -lncurses

# Gestion des fichiers à compiler et de l'exécutable
SRCS := $(shell find $(SRC_DIR) -name *.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
TARGET ?= tetris

# Gestion des commandes de création de repertoire et suppression
MKDIR_P ?= mkdir -p
RM_R ?= rm -r

# Règles de création de la cible
$(BUILD_DIR)/$(TARGET) : $(OBJS)
	@echo "Génération de la cible : $@"
	@$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Règles de compilations des fichiers de dépendances
$(BUILD_DIR)/$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@echo "Compilation : $<"
	@$(MKDIR_P) $(BUILD_DIR)/$(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

# Règles de nettoyage
.PHONY : clean
clean :
	$(RM_R) $(BUILD_DIR)

# Inclusion des dépendances
-include $(DEPS)