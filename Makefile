# Variables
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pedantic -g
PROG = tienlen
SRC = $(PROG).c cardgame.c
HDR = cardgame.h
OBJ = $(SRC:.c=.o)

# Targets
$(PROG): $(OBJ)
$(OBJ): $(HDR)
TAGS: $(SRC) $(HDR)
	etags $^
.PHONY: clean
clean:
	$(RM) $(PROG) $(OBJ)
