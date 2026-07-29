CC = gcc
CFLAGS = -Wall -Werror
LIBS = -lpthread

OBJ_DIR = obj
SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET = main

all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -o $@ -c $<

clean:
	rm -rf $(TARGET) $(OBJS)
