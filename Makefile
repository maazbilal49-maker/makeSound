CC = gcc
CFLAGS = -Wall -Wextra -O2
LIBFLAGS = -lm
SDL2FLAGS = $(shell sdl2-config --cflags --libs)

DEST = makeSound
ORG = main.c

all: $(DEST)

$(DEST): $(ORG)
	$(CC) $(CFLAGS) -o $(DEST) $(ORG) $(LIBFLAGS) $(SDL2FLAGS)

clean:
	rm -f $(DEST)