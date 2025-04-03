# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L

# Directories (relative to the test directory)
SRC_DIR = ../
TEST_DIR = .
OBJ_DIR = object
BIN_DIR = binary

# Library sources: all .c files in SRC_DIR except main.c
LIB_SRCS = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SRCS))
LIB      = $(BIN_DIR)/libtagger.a

# Test sources: all .c files in TEST_DIR
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
# For each test source file, the corresponding executable will have the same base name.
TEST_EXES = $(patsubst $(TEST_DIR)/%.c, $(BIN_DIR)/%, $(TEST_SRCS))

.PHONY: all clean

# Default target: build the library and all test executables.
all: $(LIB) $(TEST_EXES)

# Build the static library from library object files.
$(LIB): $(LIB_OBJS)
	@mkdir -p $(BIN_DIR)
	ar rcs $(LIB) $(LIB_OBJS)

# Compile a source file from SRC_DIR into an object file.
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile a test source file from TEST_DIR into an object file.
# The -I flag adds SRC_DIR to the include path so test files can include your headers.
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

# Link a test executable by combining its test object with the library.
$(BIN_DIR)/%: $(OBJ_DIR)/%.o $(LIB)
	@mkdir -p $(BIN_DIR)
	$(CC) $< $(LIB) -o $@

# Clean up object and binary directories.
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
