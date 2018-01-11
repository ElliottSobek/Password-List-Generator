# Runs rules for given commands.
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
SHELL=/bin/bash

CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -O0 -g -D_POSIX_C_SOURCE=200809L

LDLIBS = -lm -pthread

.PHONY: all test asm-instr clean

all: pwd-list-gen

pwd-list-gen: pwd-list-gen.o

test: pwd-list-gen.o
	$(CC) -Wall -Wextra -Wpedantic -std=c99 -O0 -g $< $(LDLIBS) -o test.exe
	chmod 0331 test.exe

pwd-list-gen.o: pwd-list-gen.c

asm-instr:
	$(CC) $(CFLAGS) pwd-list-gen.c $(LDLIBS) -S
	wc -l pwd-list-gen.s

clean:
	$(RM) *.o pwd-list-gen
