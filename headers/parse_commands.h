#ifndef PARSE_COMMANDS
#define PARSE_COMMANDS

#include "simple_globals.h"

typedef enum parsing_errors {Command_Not_Found = 1 , Not_Enough_Args = 2} parsing_errors;

void parse_command(char *input);

#endif