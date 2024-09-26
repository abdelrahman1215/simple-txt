#ifndef PARSE_COMMANDS
#define PARSE_COMMANDS

#include "../c_datastructures/headers/linked_list.h"
#include "simple_globals.h"

typedef void (command_func) (char **tokens);

typedef struct command_info{
    char *command;
    size_t command_len;
    //specifies the number of tokens that should follow a command and be given to it
    unsigned short token_no;

    command_func *command_exec;
}command_info;

typedef enum parsing_errors {Command_Not_Found = 1 , Not_Enough_Args = 2} parsing_errors;

void parse_command(char *input);
linked_list *get_nearest_command_list(char *tok);

#endif