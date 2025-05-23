PROJECT = test
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
EXECUTABLES = $(patsubst %.c, %, $(SOURCES))

CC = gcc
CFLAGS = -Wall -Wextra -g -fsanitize=address -fsanitize=leak -fsanitize=undefined

default: $(PROJECT)

clean:
	rm $(PROJECT)

$(PROJECT): $(SOURCES) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $(PROJECT) $(SOURCES)
