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
        "inode/x-empty",//an empty file with no extesions
        "inode/symlink"// ??
    };

    for(int i = 0 ; i < arr_size(valid_types) ; i++){
        if(strncmp(type , valid_types[i] , strlen(valid_types[i])) == 0) return true;
    }

    return false;
}

void open_file(const char *file_name){
    if(file_name[0] == '\000') return ;
    if(is_dir(file_name)) {
        if(stdscr == NULL){
            init_display_attrs();
            init_values();
            start_display();
        }
        
        open_dir(file_name , stdscr , 1 , Screen_Width - 1 , 2 , Screen_Height);

        return;
    }else{
        magic_t cookie = magic_open(MAGIC_MIME_TYPE);
        if(cookie == NULL) return;

        if(magic_load(cookie , NULL) != 0){
            magic_close(cookie);
            return;
        }

        memset(Last_Opened_Path , 0 , PATH_MAX + 1);
        size_t path_len = strlen(file_name);
        size_t dir_path_len;
        for(dir_path_len = path_len ; dir_path_len > 0 ; dir_path_len--){
            if(file_name[dir_path_len - 1] == '\\' || file_name[dir_path_len - 1] == '/') break;
        }

        strncpy(Last_Opened_Path , file_name , dir_path_len);
        
        const char *type = magic_file(cookie , file_name);
        if(!is_valid_type(type)){
            
            if(strncmp(type , "cannot" , 6) != 0){
                loading_error(file_name , Invalid_File_Type);
                //custom_msg(Message , type);
                magic_close(cookie);


                return;
            }
        }

        magic_close(cookie);
    }

    loading_err error;
    simple_file *tmp = load_file(file_name , true , true , &error);

    if(tmp != NULL){
        if(Current_File != NULL){
            destroy_simple_file(Current_File);
        }

        Current_File = tmp;
    }

    loading_error(file_name , error);

    memset(Last_Opened_Path , 0 , PATH_MAX + 1);
    size_t path_len = strlen(file_name);
    size_t dir_path_len;
    for(dir_path_len = path_len ; dir_path_len > 0 ; dir_path_len--){
        if(file_name[dir_path_len - 1] == '\\' || file_name[dir_path_len - 1] == '/') break;
    }

    strncpy(Last_Opened_Path , file_name , dir_path_len);
}