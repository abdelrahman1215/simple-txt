#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_fmt.h"
#include "open_dir.c"

#include <stdbool.h>
#include <unistd.h>
#include <magic.h>

#define arr_size(arr) sizeof(arr) / sizeof(arr[0])

bool is_valid_type(const char *type){
    char *valid_types[] = {
        "text",//any text file
        "inode/x-empty"//an empty file with no extesions
    };

    for(int i = 0 ; i < arr_size(valid_types) ; i++){
        if(strncmp(type , valid_types[i] , strlen(valid_types[i])) == 0) return true;
    }

    return false;
}

void open_file(char *file_name){
    if(file_name[0] == '\000') return ;
    if(is_dir(file_name)) {
        open_dir(file_name);

        return;
    }else{
        magic_t cookie = magic_open(MAGIC_MIME_TYPE);
        if(cookie == NULL) return;

        if(magic_load(cookie , NULL) != 0){
            magic_close(cookie);
        }

        const char *type = magic_file(cookie , file_name);
        if(!is_valid_type(type)){
            if(strncmp(type , "cannot" , 6) != 0){
                loading_error(file_name , Invalid_File_Type);
                return;
            }
        }

        magic_close(cookie);
    }

    loading_err error;
    simple_file *tmp = load_file(file_name , true , &error);

    if(Current_File != NULL){
        if(!simple_file_changes_saved(Current_File)){
            custom_msg(Error , "save the current file first");
            return ;
        }
    }

    if(tmp != NULL){
        if(Current_File != NULL){
            destroy_simple_file(Current_File);
        }

        Current_File = tmp;
    }

    loading_error(file_name , error);
}