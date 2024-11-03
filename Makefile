CC = gcc
CFLAGS = -Wall -Wextra -I./include
LDFLAGS = -lcmark

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = $(BIN_DIR)/markdown-site-gen

.PHONY: all clean install test

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

install:
	install -d $(DESTDIR)/usr/local/bin
	install $(TARGET) $(DESTDIR)/usr/local/bin/markdown-site-gen

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)