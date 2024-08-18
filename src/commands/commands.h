#ifndef COMMANDS
#define COMMANDS

//command : "w"
//tokens_no : 0
void *write_to_current_file(char **args);

//command : "q"
//tokens_no : 0
void *quit(char **args);

//command : "wq"
//tokens_no : 0
void *write_and_quit(char **args);

//command : "edit"
//tokens_no : 1
void *edit_file(char **args);

#endif