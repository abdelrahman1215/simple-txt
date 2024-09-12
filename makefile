CC = gcc
C_Flags = -g3
Data_Structures_Path = c_datastructures/bin/

test : simple_file.o error_format.o msg_format.o parse_commands.o parse_term_args.o display.o datastructs
	$(CC) $(C_Flags) -o test test.c bin/*.o -lpdcurses -L$(Data_Structures_Path) -ldynamic_array -llinked_list -lhashmap

simple_str.o : src/simple_str.c bin/
	$(CC) $(C_Flags) -c src/simple_str.c -o bin/simple_str.o

simple_file.o : src/file_processing/load_file.c src/file_processing/edit_file.c bin/ simple_str.o div_str.o
	$(CC) $(C_Flags) -c src/file_processing/simple_file.c -o bin/simple_file.o

error_format.o : src/error_format.c bin/
	$(CC) $(C_Flags) -c src/error_format.c -o bin/error_format.o

msg_format.o : src/msg_format.c bin/ 
	$(CC) $(C_Flags) -c src/msg_format.c -o bin/msg_format.o

parse_commands.o : src/commands/parse_commands.c bin/
	$(CC) $(C_Flags) -c src/commands/parse_commands.c -o bin/parse_commands.o

parse_term_args.o : src/term_args/parse_term_args.c bin/
	$(CC) $(C_Flags) -c src/term_args/parse_term_args.c -o bin/parse_term_args.o

display.o : src/display/display.c bin/
	$(CC) $(C_Flags) -c src/display/display.c -o bin/display.o

div_str.o : src/div_str.c bin/
	$(CC) $(C_Flags) -c src/div_str.c -o bin/div_str.o

datastructs :
	$(MAKE) -C c_datastructures libs

bin/ : 
	$(shell mkdir bin)