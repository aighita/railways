CC=gcc
CFLAGS=-I./include

all: rebuild
	./tema3

tema3:
	$(CC) $(CFLAGS) -o tema3 tema3.c ./lib/*.c

build: tema3

rebuild: clean build

clean:
	rm tema3

valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema3

check:
	./run_tests.sh
