CC = gcc
CFLAGS = -Werror -Wall -Wextra

# Setup a phony target to build everything
.PHONY: all

all:  assign2

assign2: a2lib.h a2lib.c

%: %.c
	$(CC) $(CFLAGS) -o $@ $^
