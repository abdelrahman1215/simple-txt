#ifndef PARSE_TERM_ARGS
#define PARSE_TERM_ARGS

typedef enum parsing_term_errors {File_Is_Dir = 0 , Option_Not_Found = 1 , Flag_Not_Found = 2 , Too_Few_Args = 3 , Too_Many_Files = 4} parsing_term_errors;

void parse_term_args(int argc , char **argv);

#endif