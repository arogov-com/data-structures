PROJECT = test
SOURCES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
EXECUTABLES = $(patsubst %.c, %, $(SOURCES))

CC = gcc
CFLAGS = -Wall -g

default: $(PROJECT)

clean:
	rm $(PROJECT)

$(PROJECT): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $(PROJECT) $(SOURCES)
