# Variables
CC = gcc
CFLAGS = -Wall -pedantic -pthread
LDFLAGS = -lsystemd

SERVER_SRC_DIR = tvsctl-srv/src
CLIENT_SRC_DIR = tvsctl-cli/src

SERVER_BUILD_DIR = tvsctl-srv/bin
CLIENT_BUILD_DIR = tvsctl-cli/bin

SERVER_SRC = $(SERVER_SRC_DIR)/tvsctld.c $(SERVER_SRC_DIR)/handler/tvsctld_commands.c $(SERVER_SRC_DIR)/parser/tvsctld_parser.c $(SERVER_SRC_DIR)/utils/utils.c
CLIENT_SRC = $(CLIENT_SRC_DIR)/tvsctl.c 

CLIENT_TARGET = $(CLIENT_BUILD_DIR)/tvsctl
SERVER_TARGET = $(SERVER_BUILD_DIR)/tvsctld

# Rules
all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_SRC)
	mkdir -p $(CLIENT_BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SERVER_TARGET): $(SERVER_SRC) $(SERVER_SRC_DIR)/handler/tvsctld_commands.h $(SERVER_SRC_DIR)/parser/tvsctld_parser.h $(SERVER_SRC_DIR)/utils/utils.h
	mkdir -p $(SERVER_BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(CLIENT_TARGET) $(SERVER_TARGET)

.PHONY: all clean