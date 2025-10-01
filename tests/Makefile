# Build helpers for reproducer binaries in this directory.
CC ?= gcc
ROOT_DIR := ..
BIN_DIR := bin

TEST_SOURCES := $(wildcard *.c)
TESTS := $(TEST_SOURCES:.c=)
BINARIES := $(addprefix $(BIN_DIR)/,$(TESTS))

CPPFLAGS ?=
CPPFLAGS += -I$(ROOT_DIR) -D_POSIX_C_SOURCE=200809L

CFLAGS ?= -Wall -Wextra -std=c11 -g

LIB_SOURCES := $(ROOT_DIR)/tagger_lib.c $(ROOT_DIR)/util_lib.c

.PHONY: all clean test test-% test-all verify-test-name

all: $(BINARIES)

$(BIN_DIR):
	mkdir -p $@

$(BIN_DIR)/%: %.c $(LIB_SOURCES) | $(BIN_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) $< $(LIB_SOURCES) $(LDFLAGS) $(LDLIBS) -o $@
	ln -sf $@ $*

test: verify-test-name
	@$(MAKE) --no-print-directory test-$(TEST)

test-%: $(BIN_DIR)/%
	@printf 'Running %s...\n' "$*"
	@./$*

test-all: $(BINARIES)
	@for test in $(TESTS); do \
		printf 'Running %s...\n' "$$test"; \
		./"$$test" || exit $$?; \
		printf '\n'; \
	done

verify-test-name:
	@if [ -z "$(TEST)" ]; then \
		echo 'Usage: make test TEST=<test_name>'; \
		echo 'Available tests: $(TESTS)'; \
		exit 2; \
	fi; \
	if ! printf '%s\n' $(TESTS) | grep -qx "$(TEST)"; then \
		echo "Unknown test '$(TEST)'"; \
		echo 'Available tests: $(TESTS)'; \
		exit 2; \
	fi

clean:
	rm -rf $(BIN_DIR)
	rm -f $(TESTS)
