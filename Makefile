# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -Werror -O2

# Directories
BUILD_DIR = build

# Source and object files
SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = $(BUILD_DIR)/sh

.PHONY: all clean

all: $(TARGET)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

dbg: $(TARGET)
	gdb $(TARGET)
clean:
	rm -rf $(BUILD_DIR)