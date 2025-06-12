# Compiler and flags
CC := gcc
CFLAGS := -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

# Directories
SRC_DIR := .
OBJ_DIR := object
BIN_DIR := binary

# Static library settings
LIB_NAME := tagger
LIB := $(BIN_DIR)/lib$(LIB_NAME).a
LIB_SRCS := $(wildcard $(SRC_DIR)/*_lib.c)
LIB_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))

LDLIBS += -lm

# Script/executable sources: all .c files except *_lib.c
SCRIPT_SRCS := $(filter-out $(LIB_SRCS),$(wildcard $(SRC_DIR)/*.c))
SCRIPT_EXES := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%,$(SCRIPT_SRCS))

.PHONY: all clean
.SECONDARY:

# Default target: build library and all scripts
all: $(LIB) $(SCRIPT_EXES)

# Build static library
$(LIB): $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	ar rcs $@ $^

# Build scripts/executables
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $< $(LIB) $(LDLIBS) -o $@

# Compile sources to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

