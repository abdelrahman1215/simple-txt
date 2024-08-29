#include "../../headers/simple_globals.h"
#include "../../headers/error_format.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_format.h"

#include <stdlib.h>

void write_to_current_file(char **args){
    save_file(Current_File);

    return ;
}

void quit(char **args){
    destroy_simple_file(Current_File);
    exit(0);

    return ;
}

void write_and_quit(char **args){
    write_to_current_file(args);
    quit(args);

    return ;
}

void edit_file(char **args){
    loading_err error;
    simple_file *tmp = load_file(args[0] , &error);

    if(error == OK || error == File_Not_Found){
        save_file(Current_File);
        destroy_simple_file(Current_File);
        Current_File = tmp;

        if(error == File_Not_Found){
            loading_msg(args[0] , File_Not_Found);
        }

        Txt_Start_Line = Col_Pos = Line_Pos = 0;
    } else loading_error(args[0] , error);

    return ;
}