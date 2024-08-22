CC = gcc
C_Flags = -g3
Data_Structures_Path = c_datastructures/bin/

test : load_file.o edit_file.o error_format.o command_tree.o commands.o parse_commands.o parse_term_args.o
	$(CC) $(C_Flags) -o test test.c bin/simple_str.o bin/load_file.o bin/edit_file.o bin/command_tree.o bin/commands.o bin/parse_commands.o bin/parse_term_args.o bin/error_format.o bin/div_str.o -L$(Data_Structures_Path) -ldynamic_array -llinked_list -lhashmap

simple_str.o : src/simple_str.c bin/
	$(CC) $(C_Flags) -c src/simple_str.c -o bin/simple_str.o

load_file.o : src/file_processing/load_file.c simple_str.o bin/ dynamic_array
	$(CC) $(C_Flags) -c src/file_processing/load_file.c -o bin/load_file.o

edit_file.o : src/file_processing/edit_file.c simple_str.o div_str.o bin/ dynamic_array
	$(CC) $(C_Flags) -c src/file_processing/edit_file.c -o bin/edit_file.o

error_format.o : src/error_format.c bin/ linked_list
	$(CC) $(C_Flags) -c src/error_format.c -o bin/error_format.o

command_tree.o : src/commands/command_tree.c bin/
	$(CC) $(C_Flags) -c src/commands/command_tree.c -o bin/command_tree.o

commands.o : load_file.o edit_file.o error_format.o bin/
	$(CC) $(C_Flags) -c src/commands/commands.c -o bin/commands.o

parse_commands.o : commands.o bin/
	$(CC) $(C_Flags) -c src/commands/parse_commands.c -o bin/parse_commands.o

parse_term_args.o : hashmap
	$(CC) $(C_Flags) -c src/term_args/parse_term_args.c -o bin/parse_term_args.o

div_str.o : src/div_str.c bin/
	$(CC) $(C_Flags) -c src/div_str.c -o bin/div_str.o

dynamic_array :
	$(MAKE) -C c_datastructures dynamic_array

linked_list :
	$(MAKE) -C c_datastructures linked_list

hashmap :
	$(MAKE) -C c_datastructures hashmap

bin/ : 
	$(shell mkdir bin)