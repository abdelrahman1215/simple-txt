CC = gcc
Data_Structures_Path = c_datastructures/bin/

test : load_file.o edit_file.o error_format.o command_tree.o commands.o
	$(CC) -g3 test.c bin/simple_str.o bin/load_file.o bin/edit_file.o bin/command_tree.o bin/commands.o bin/error_format.o bin/div_str.o -o test -L$(Data_Structures_Path) -ldynamic_array -llinked_list

simple_str.o : src/simple_str.c bin/
	$(CC) -g3 -c src/simple_str.c -o bin/simple_str.o

load_file.o : src/file_processing/load_file.c simple_str.o bin/ dynamic_array
	$(CC) -g3 -c src/file_processing/load_file.c -o bin/load_file.o

edit_file.o : src/file_processing/edit_file.c simple_str.o div_str.o bin/ dynamic_array
	$(CC) -g3 -c src/file_processing/edit_file.c -o bin/edit_file.o

error_format.o : src/error_format.c bin/ linked_list
	$(CC) -g3 -c src/error_format.c -o bin/error_format.o

command_tree.o : src/command_tree.c bin/
	$(CC) -g3 -c src/command_tree.c -o bin/command_tree.o

commands.o : load_file.o edit_file.o error_format.o bin/
	$(CC) -g3 -c src/commands/commands.c -o bin/commands.o

div_str.o : src/div_str.c bin/
	$(CC) -g3 -c src/div_str.c -o bin/div_str.o

dynamic_array :
	$(MAKE) -C c_datastructures dynamic_array

linked_list :
	$(MAKE) -C c_datastructures linked_list

bin/ : 
	$(shell mkdir bin)