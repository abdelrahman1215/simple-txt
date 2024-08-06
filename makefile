CC = gcc
Data_Structures_Path = c_datastructures/bin/

test : simple_str.o load_file.o edit_file.o div_str.o
	$(CC) -g3 test.c bin/simple_str.o bin/load_file.o bin/edit_file.o bin/div_str.o -o test -L$(Data_Structures_Path) -ldynamic_array

simple_str.o : src/simple_str.c bin/
	$(CC) -g3 -c src/simple_str.c -o bin/simple_str.o

load_file.o : src/files/load_file.c simple_str.o bin/ dynamic_array
	$(CC) -g3 -c src/files/load_file.c -o bin/load_file.o

edit_file.o : src/files/edit_file.c simple_str.o bin/ dynamic_array
	$(CC) -g3 -c src/files/edit_file.c -o bin/edit_file.o

div_str.o : src/div_str.c bin
	$(CC) -g3 -c src/div_str.c -o bin/div_str.o

dynamic_array :
	$(MAKE) -C c_datastructures dynamic_array

bin/ : 
	$(shell mkdir bin)