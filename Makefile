TARGET = jsonParser

CC = gcc
CFLAGS = -Wall -Wextra -g -Iinc

SRC_DIR = src
INCLUDE_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

# main.c
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))
DEPS = $(wildcard $(INCLUDE_DIR)/*.h)

$(shell mkdir  $(OBJ_DIR) $(BIN_DIR)) 

all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

rebuild: clean all

.PHONY: all clean rebuild

