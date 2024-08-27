#include "../../headers/simple_globals.h"
#include "../../headers/error_format.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_format.h"

//tokens : 1
void open_file(char *file_name){
    loading_err error;
    simple_file *tmp = load_file(file_name , &error);

    if(error == OK || error == File_Not_Found){
        save_file(Current_File);
        destroy_simple_file(Current_File);
        Current_File = tmp;

        if(error == File_Not_Found){
            loading_msg(file_name , error);
        }

        Txt_Start_Line = 0;
    }else{
        loading_error(file_name , error);
    }
}