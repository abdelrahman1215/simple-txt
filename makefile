CC = gcc

test : fat_str.o
	$(CC) test.c bin/fat_str.o -o test

fat_str.o : src/fat_str.c bin/
	$(CC) -c src/fat_str.c -o bin/fat_str.o

bin/ : 
	$(shell mkdir bin)