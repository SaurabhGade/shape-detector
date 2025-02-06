CC=gcc
DIR=$(shell pwd)
LDFLAGS=-lm -lc
CFLAGS=-g -g3 -Wall -Wextra
TARGET=main

$(TARGET): $(DIR)/main.c $(DIR)/image.h
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean: 
	rm -fv $(TARGET)
