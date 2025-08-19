# Compilar versión C (con structs)
CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -O2 -D_POSIX_C_SOURCE=199309L

SRC = main.c persona.c generador.c monitor.c utils.c
OBJ = $(SRC:.c=.o)
EXEC = programa_c

.PHONY: all clean run

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	@./$(EXEC) 50000   # cambia N si querés

clean:
	rm -f $(OBJ) $(EXEC)
