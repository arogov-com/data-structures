PROJECT = test
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
EXECUTABLES = $(patsubst %.c, %, $(SOURCES))

CC = gcc
CFLAGS = -Wall -g -fsanitize=address -fsanitize=leak

default: $(PROJECT)

clean:
	rm $(PROJECT)

$(PROJECT): $(SOURCES) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $(PROJECT) $(SOURCES)
