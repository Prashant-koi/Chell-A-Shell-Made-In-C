CC = gcc
CFLAGS = -Wall -Wextra -g

all: chell

chell: chell.c chell_extenshions.c
	$(CC) $(CFLAGS) -o chell chell.c chell_extensions.c

clean:
	rm -f chell