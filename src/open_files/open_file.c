#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_fmt.h"

#include <unistd.h>

void open_file(char *file_name){
    if(file_name[0] == '\000') return ;

    loading_err error;
    simple_file *tmp = load_file(file_name , true , &error);

    if(tmp != NULL){
        if(Current_File != NULL){
            save_file(Current_File);
            destroy_simple_file(Current_File);
        }
        
        Current_File = tmp;
    }

    if(error == OK || error == File_Not_Found){
        if(error == File_Not_Found){
            loading_msg(file_name , error);
        }
    }else{
        loading_error(file_name , error);
    }
}