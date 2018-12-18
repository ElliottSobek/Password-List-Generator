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

SHELL := /bin/bash

CC := gcc

CFLAGS := -Wall -Wextra -pedantic -std=c99 -D_POSIX_C_SOURCE=200809L -g

LDLIBS := -lm -pthread

SRC := src

OBJECTS := pwd-list-gen.o

VPATH := $(SRC)

# ifeq ($(MAKECMDGOALS),)
# override CFLAGS += -g
# else ifeq ($(MAKECMDGOALS),debug)
# override CFLAGS += -g
# else ifeq ($(MAKECMDGOALS),profile)
# override CFLAGS += -pg
# else
# override CFLAGS += -O3
# endif

.PHONY: debug profile production test clean

debug: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o pwd-list-gen-debug

profile: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o pwd-list-gen-profile

production: $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LDLIBS) -o pwd-list-gen

test:
	./test.bash

$(OBJECTS):

clean:
	$(RM) $(OBJECTS) pwd-list-gen-debug pwd-list-gen-profile pwd-list-gen
