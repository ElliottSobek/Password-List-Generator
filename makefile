# Generates all possible character combinations for a given character set and
# length to a file.
# Copyright (C) 2017  Elliott Sobek
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

CC = gcc

CFLAGS = -x c -Wall -Wextra -Wpedantic -std=gnu99 -O0 -g

LDLIBS = -lm -pthread

.PHONY: all clean

all: pwd-list-gen

pwd-list-gen: pwd-list-gen.o
	$(CC) $(CFLAGS) $< $(LDLIBS) -o $@

pwd-list-gen.o: pwd-list-gen.c

asm-instr:
	$(CC) $(CFLAGS) pwd-list-gen.c $(LDLIBS) -S

clean:
	rm *.o
