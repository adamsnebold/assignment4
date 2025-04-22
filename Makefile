CC=gcc --std=c99 -g

all: test

test: test.c hash_table.o
	$(CC) test.c hash_table.o -o test

hash_table.o: hash_table.c hash_table.h node.h
	$(CC) -c hash_table.c -o hash_table.o

clean:
	rm -rf *.dSYM/
	rm -f *.o test
