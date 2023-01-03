CC ?= gcc
CFLAGS ?= -Wall -Wextra
LDFLAGS ?=

VPATH=src

objects := builtin.o main.o parser.o processus.o

.PHONY: clean

minishell: $(objects)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

builtin.o: builtin.c builtin.h processus.h
	$(CC) $(CFLAGS) -c $<

main.o: main.c parser.h processus.h
	$(CC) $(CFLAGS) -c $<

parser.o: parser.c parser.h processus.h
	$(CC) $(CFLAGS) -c $<

processus.o: processus.c processus.h builtin.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(objects) *.gch minishell
