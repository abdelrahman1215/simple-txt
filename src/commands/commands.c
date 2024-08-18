#include "../../headers/simple_globals.h"
#include "../../headers/error_format.h"
#include "../../headers/simple_file.h"

#include <stdlib.h>

//command : "w"
//tokens_no : 0
void *write_to_current_file(char **args){
    save_file(current_file);

    return NULL;
}

//command : "q"
//tokens_no : 0
void *quit(char **args){
    destroy_simple_file(current_file);
    exit(0);

    return NULL;
}

//command : "wq"
//tokens_no : 0
void *write_and_quit(char **args){
    write_to_current_file(args);
    quit(args);

    return NULL;
}

//command : "edit"
//tokens_no : 1
void *edit_file(char **args){
    loading_err error;
    simple_file *tmp = load_file(args[0] , &error);

    if(error == OK){
        save_file(current_file);
        destroy_simple_file(current_file);
        current_file = tmp;
    } else loading_error(args[0] , error);

    return NULL;
}