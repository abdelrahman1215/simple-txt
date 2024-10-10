#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_fmt.h"

#include <unistd.h>

void open_file(char *file_name){
    if(file_name[0] == '\000') return ;

    loading_err error;
    Current_File = load_file(file_name , true , &error);

    if(error == OK || error == File_Not_Found){
        if(error == File_Not_Found){
            loading_msg(file_name , error);
        }else{
            for(size_t i = strlen(file_name) ; i > 0 ; i--){
                if(file_name[i - 1] == '/' || file_name[i - 1] == '\\'){
                    file_name[i - 1] = '\000';

                    break;
                }
            }

            if(file_name[0] != '\000') chdir(file_name);
        }
    }else{
        loading_error(file_name , error);
    }
}