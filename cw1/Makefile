CC = gcc
CFLAGS = -g -Wall -pedantic
SRC = bach.c structs/simple_command.c structs/parent_command.c structs/command.c utils/alloc.c utils/io.c parser/parser.c handler/handler.c
TARGET = bach

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) $(CFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)
