#include "../../headers/simple_globals.h"
#include "../../headers/error_format.h"
#include "../../headers/simple_file.h"

//tokens : 1
void open_file(char *file_name){
    loading_err error;
    simple_file *tmp = load_file(file_name , &error);

    if(error == OK){
        save_file(Current_File);
        destroy_simple_file(Current_File);
        Current_File = tmp;
    }else if(error == Is_Dir){
        term_arg_error(file_name , File_Is_Dir);
    }else{
        loading_error(file_name , error);
    }

    return ;
}