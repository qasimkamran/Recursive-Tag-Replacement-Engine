# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

# Directories
SRC_DIR = .
OBJ_DIR = object
BIN_DIR = binary

# All source files in the source directory
SRCS = $(wildcard $(SRC_DIR)/*.c)

# main.c is the source for the executable
MAIN_SRC = main.c

# Library sources are all sources except main.c
LIB_SRCS = $(filter-out $(MAIN_SRC), $(SRCS))

# Object files for main and library sources (placed in OBJ_DIR)
MAIN_OBJ = $(OBJ_DIR)/main.o
LIB_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))

# Final targets: executable and static library (placed in BIN_DIR)
EXE = $(BIN_DIR)/main
LIB = $(BIN_DIR)/libtagger.a

.PHONY: all clean

# Default target builds the executable
all: $(EXE)

# Link the main object with the static library to produce the executable
$(EXE): $(MAIN_OBJ) $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $(MAIN_OBJ) $(LIB) -o $(EXE)

# Build the static library from the library object files
$(LIB): $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	ar rcs $(LIB) $(LIB_OBJS)

# Compile any .c file to an object file in OBJ_DIR
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files by removing the binary and object directories
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

