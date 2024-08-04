CC = gcc
Data_Structures_Path = c_datastructures/bin/

test : fat_str.o load_file.o
	$(CC) -g3 test.c bin/fat_str.o -o test -L$(Data_Structures_Path) -ldynamic_array

fat_str.o : src/fat_str.c bin/
	$(CC) -g3 -c src/fat_str.c -o bin/fat_str.o

load_file.o : fat_str.o bin/ dynamic_array
	$(CC) -c src/load_file.c -o bin/load_file.o

dynamic_array :
	$(MAKE) -C c_datastructures dynamic_array

bin/ : 
	$(shell mkdir bin)