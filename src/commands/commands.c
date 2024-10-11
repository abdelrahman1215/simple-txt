#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_fmt.h"
#include "../../headers/open_files.h"

#include <stdlib.h>

//command : "w"
//tokens_no : 0
void write_to_current_file(char **args){
    if(simple_file_changes_saved(Current_File)) return ;

    save_file(Current_File);
    wrote_into_file_msg(simple_file_get_name(Current_File));

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
    open_file(args[0]);

    return ;
}