CC = gcc

CFLAGS = -Wall -Wextra -std=gnu99 -g

.PHONY: all clean

all: pwd-list-gen

pwd-list-gen: pwd-list-gen.o
	$(CC) $(CFLAGS) pwd-list-gen.o -o a

pwd-list-gen.o: pwd-list-gen.c

clean:
	rm *.o
