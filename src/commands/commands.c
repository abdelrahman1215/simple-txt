#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_fmt.h"

#include <stdlib.h>

//command : "w"
//tokens_no : 0
void write_to_current_file(char **args){
    save_file(Current_File);

    return ;
}

//command : "q"
//tokens_no : 0
void quit(char **args){
    Quit = true;

    return ;
}

//command : "wq"
//tokens_no : 0
void write_and_quit(char **args){
    write_to_current_file(args);
    quit(args);

    return ;
}

//command : "edit"
//tokens_no : 1
void edit_file(char **args){
    loading_err error;
    simple_file *tmp = load_file(args[0] , true , &error);

    if(error == OK || error == File_Not_Found){
        save_file(Current_File);
        destroy_simple_file(Current_File);
        Current_File = tmp;

        if(error == File_Not_Found){
            loading_msg(args[0] , File_Not_Found);
        }
    } else loading_error(args[0] , error);

    return ;
}