#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_fmt.h"
#include "open_dir.c"

#include <unistd.h>

void open_file(char *file_name){
    if(file_name[0] == '\000') return ;

    loading_err error;
    simple_file *tmp = load_file(file_name , true , &error);

    if(tmp != NULL){
        if(Current_File != NULL){
            if(simple_file_changes_saved(Current_File)){
                destroy_simple_file(Current_File);
            }else{
                custom_msg(Error , "save the current file first");
            }
        }

        Current_File = tmp;
    }

    if(error == Is_Dir) open_dir(file_name);
    loading_error(file_name , error);
}