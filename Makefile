# Makefile para el Controlador Híbrido C/ASM

# Compiladores y herramientas
CC = gcc
ASM = nasm
AR = ar
RM = rm -f

# Flags de compilación
CFLAGS = -Wall -Wextra -O2 -I./include
ASMFLAGS = -f elf64
LDFLAGS = -lm

# Directorios
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
LIB_DIR = lib
EXAMPLES_DIR = examples

# Archivos fuente
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.asm)
C_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_SOURCES))
ASM_OBJECTS = $(patsubst $(SRC_DIR)/%.asm,$(OBJ_DIR)/%.o,$(ASM_SOURCES))
OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

# Nombre de la biblioteca
LIB_NAME = libcontrolador.a

# Regla por defecto
all: directories $(LIB_DIR)/$(LIB_NAME) examples

# Crear directorios necesarios
directories:
	@mkdir -p $(OBJ_DIR) $(LIB_DIR)

# Compilar archivos C
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar archivos ASM
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Crear la biblioteca
$(LIB_DIR)/$(LIB_NAME): $(OBJECTS)
	$(AR) rcs $@ $^

# Compilar ejemplos
examples: $(LIB_DIR)/$(LIB_NAME)
	$(CC) $(CFLAGS) $(EXAMPLES_DIR)/ejemplo_uso.c -L$(LIB_DIR) -lcontrolador $(LDFLAGS) -o $(EXAMPLES_DIR)/ejemplo_uso

# Limpiar archivos generados
clean:
	$(RM) $(OBJ_DIR)/*.o $(LIB_DIR)/$(LIB_NAME) $(EXAMPLES_DIR)/ejemplo_uso

# Instalar la biblioteca
install: $(LIB_DIR)/$(LIB_NAME)
	@echo "Instalando biblioteca..."
	@mkdir -p /usr/local/lib
	@mkdir -p /usr/local/include
	@cp $(LIB_DIR)/$(LIB_NAME) /usr/local/lib/
	@cp $(INCLUDE_DIR)/*.h /usr/local/include/
	@ldconfig

.PHONY: all directories clean install examples
