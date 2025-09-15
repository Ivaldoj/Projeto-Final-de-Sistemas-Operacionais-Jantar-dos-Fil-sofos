# Compila tudo que estiver em src/
CC=gcc
CFLAGS=-O2 -Wall -pthread
TARGET=filosofos

SRC=$(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
