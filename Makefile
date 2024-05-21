CC = gcc
CFLAGS = -Wall -Wextra -pthread
LDFLAGS = -pthread

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
MAIN = $(BUILD_DIR)/main

TEST_SRCS = $(TEST_DIR)/basic_alloc_free.c \
#            $(TEST_DIR)/calloc_realloc.c \
#            $(TEST_DIR)/thread_safety.c
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))
TESTS = $(patsubst $(TEST_DIR)/%.c,$(BUILD_DIR)/%,$(TEST_SRCS))

.PHONY: all clean test

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

$(BUILD_DIR)/%: $(BUILD_DIR)/%.o $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

test: $(TESTS)
	@for test in $(TESTS); do \
		echo "Running $$test"; \
		./$$test; \
	done

clean:
	rm -rf $(BUILD_DIR)