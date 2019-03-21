.PHONY: clean

# Compiler & options
CC=g++
CFLAGS=-std=c++11 -O2 -pthread -Wall -Wextra -Wshadow -Wconversion -fsanitize=address -fsanitize=undefined
CFLAGS+=$(DFLAGS)

# File directory
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src

# Files
_TRACEROUTE=traceroute socket icmp common
_TRACEROUTE_OBJ=traceroute.o socket.o icmp.o common.o

# Directed Files
TRACEROUTE=$(patsubst %, $(BIN_DIR)/%, $(_TRACEROUTE))
TRACEROUTE_OBJ=$(patsubst %, $(OBJ_DIR)/%, $(_TRACEROUTE_OBJ))

# Default target: all
default: clean all

# Make all
all: $(TRACEROUTE)

# Make traceroute
traceroute: $(TRACEROUTE)

$(TRACEROUTE): $(TRACEROUTE_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Objective file dependency
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

# Clean middle files
clean:
	rm -f $(OBJ_DIR)/*
	rm -f $(BIN_DIR)/*
