CC=gcc
INCLUDE=include
SRC_DIR=src
all: src

.PHONY: src

src:
	$(CC) -I $(INCLUDE) -c $(SRC_DIR)/Server.c
	$(CC) -o server Server.o
	
clean:
	rm -rf *.o
	rm -rf server