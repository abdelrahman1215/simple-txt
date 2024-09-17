#include "../../headers/simple_globals.h"
#include "../../headers/error_format.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_format.h"

//tokens : 1
void open_file(char *file_name){
    loading_err error;
    Current_File = load_file(file_name , true , &error);

    if(error == OK || error == File_Not_Found){
        if(error == File_Not_Found){
            loading_msg(file_name , error);
        }
    }else{
        loading_error(file_name , error);
    }
}