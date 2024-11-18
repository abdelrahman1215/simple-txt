#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/simple_str.h"
#include "simple_file_struct.c"

#include <sys/stat.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

bool is_dir(const char *file_name){
    struct stat file_stat;

    stat(file_name , &file_stat);

    return (bool) S_ISDIR(file_stat.st_mode);
}

#define buff_size (8 * 1024 * 1024) - 1

void free_simple_str(simple_str **str_ptr_ptr){
    destroy_simple_str(*str_ptr_ptr);
    free(str_ptr_ptr);
}

simple_file *__allocate_simple_file__(const char *file_name){
    simple_file *ret = malloc(sizeof(simple_file));
    if(ret == NULL){
        return NULL;
    }

    size_t len;
    if(file_name != NULL){
        len = strnlen(file_name , 9223372036854775807);
        ret -> file_name = calloc(len + 1 , sizeof(char));
        if(ret -> file_name == NULL){
            free(ret);

            return NULL;
        }

        strncpy(ret -> file_name , file_name , len);
    }else{
        ret -> file_name = NULL;
    }


    ret -> lines = new_dynamic_array(sizeof(simple_str *) , (free_func *)free_simple_str);
    if(ret -> lines == NULL){
        if(file_name)
        free(ret -> file_name);
        free(ret);

        return NULL;
    }
    
    ret -> changes_stack = new_linked_list();
    if(ret -> changes_stack == NULL){
        destroy_dynamic_array(ret -> lines);
        free(ret -> file_name);
        free(ret);

        return NULL;
    }

    ret -> undone_stack = new_linked_list();
    if(ret -> undone_stack == NULL){
        destroy_dynamic_array(ret -> lines);
        destroy_linked_list(ret -> changes_stack);

        free(ret -> file_name);
        free(ret);

        return NULL;
    }

    return ret;
}

char *__get_file_text__(const char *file_name , bool create_if_not_found , loading_err *get_err , simple_file *file_ptr){
    FILE *target_file = fopen(file_name , "r");
    bool created_new_file = false;
    if(target_file == NULL){
        *get_err = File_Not_Found;
        if(create_if_not_found == false){
            return NULL;
        }

        target_file = fopen(file_name , "w+");

        if(target_file == NULL){
            *get_err = Invalid_File_Name;

            return NULL;
        }else{
            fclose(target_file);
            remove(file_name);

            return "";
        }

        created_new_file = true;
    }

    char *buff = calloc(buff_size + 1 , sizeof(char));
    if(buff == NULL){
        destroy_dynamic_array(file_ptr -> lines);
        free(file_ptr -> file_name);
        free(file_ptr);

        *get_err = Alloc_Err;
        return NULL;
    }

    simple_str *file_buff = new_simple_str(NULL);
    if(file_buff == NULL){
        free(buff);

        destroy_dynamic_array(file_ptr -> lines);
        free(file_ptr -> file_name);
        free(file_ptr);

        *get_err = Alloc_Err;
        return NULL;
    }

    size_t read_bytes = 0;
    size_t total_read_bytes = 0;

    do{
        read_bytes = fread(buff , sizeof(char) , buff_size , target_file);
        buff[read_bytes] = '\000';

        simple_str_add(file_buff , buff , total_read_bytes);

        total_read_bytes += read_bytes;
    }while(read_bytes == buff_size);

    free(buff);

    char *ret = simple_str_get_string(file_buff);
    if(ret == NULL){
        free(file_buff);

        destroy_dynamic_array(file_ptr -> lines);
        free(file_ptr -> file_name);
        free(file_ptr);

        *get_err = Alloc_Err;
        return NULL;
    }

    destroy_simple_str(file_buff);

    fclose(target_file);

    if(ret[0] == '\000'){
        free(ret);

        return "";
    }

    return ret;
}

void __load_from_str__(simple_file *file_ptr , char *src){
    if(file_ptr == NULL) return ;

    simple_str *line = new_simple_str("");
    if(src == NULL){
        dynamic_array_add_element(file_ptr -> lines , &line);

        return ;
    }

    char tab[Tab_Size + 1];
    memset(tab , ' ' , Tab_Size);
    tab[Tab_Size] = '\000';

    char add[2] = {'\000' , '\000'};

    for(size_t i = 0 ; src[i] != '\000' ; i++){
        switch(src[i]){
            case '\n': 
                dynamic_array_add_element(file_ptr -> lines , &line);
                line = new_simple_str("");

                break;

            case CTRL('M'):
                break;

            case '\t':
                simple_str_add(line , tab , simple_str_get_strlen(line));

                break;

            default :
                add[0] = src[i];
                simple_str_add(line , add , simple_str_get_strlen(line));

                break;
        }
    }

    dynamic_array_add_element(file_ptr -> lines , &line);    
}

simple_file *load_from_str(const char *src , loading_err *get_err){
    if(get_err == NULL) return NULL;

    *get_err = Ok;
    simple_file *ret = __allocate_simple_file__(NULL);
    if(ret == NULL){
        *get_err = Alloc_Err;
        return NULL;
    }

    size_t len = strlen(src);
    char *copy = calloc(len + 1 , sizeof(char));
    if(copy == NULL){
        destroy_simple_file(ret);

        *get_err = Alloc_Err;
        return NULL;
    }

    strncpy(copy , src , len);
    __load_from_str__(ret , copy);

    ret -> max_reached_column = 0;
    ret -> changes_saved = false;
    ret -> column = 0;
    ret -> line = 0;

    return ret;
}

simple_file *load_file(const char *file_name , bool create_if_not_found , loading_err *get_err){
    if(get_err == NULL) return NULL;

    *get_err = Ok;
    if(file_name == NULL){
        *get_err = Invalid_Arg;

        return NULL;
    }

    if(*file_name == '\000'){
        *get_err = Invalid_Arg;

        return NULL;
    }

    if(is_dir(file_name)){
        *get_err = Is_Dir;

        return NULL;
    }

    simple_file *ret = __allocate_simple_file__(file_name);
    if(ret == NULL){
        *get_err = Alloc_Err;
        return NULL;
    }

    loading_err err = Ok;
    char *file_text = __get_file_text__(file_name , create_if_not_found , &err , ret);
    if(err != Ok){
        *get_err = err;
        if(err == Alloc_Err || err == Invalid_File_Name) return NULL;
    }

    __load_from_str__(ret , file_text);
    if(file_text != NULL) if(file_text[0] != '\000') free(file_text);

    /*the save function exits if all changes were saved
    but in the case the file doesn't exist (and is a valid path)
    we don't want it to exit because then the file won't be created by the editor
    so we set changes saved to false so that save function would try to write into that path
    thus creating that file*/

    FILE *check = fopen(file_name , "r");
    if(check == NULL){
        ret -> changes_saved = false;
    }else{
        ret -> changes_saved = true;
        fclose(check);
    }
    

    ret -> max_reached_column = 0;
    ret -> column = 0;
    ret -> line = 0;

    return ret;
}

void save_file(simple_file *file_ptr){
    if(file_ptr == NULL) return ;
    if(file_ptr -> changes_saved == true || file_ptr -> file_name == NULL) return ;

    FILE *target_file = fopen(file_ptr -> file_name , "w");
    simple_str **line_ptr;
    char *tmp;
    size_t len = 0;

    size_t line_no = simple_file_get_line_no(file_ptr);
    for(size_t i = 0 ; i + 1 < line_no ; i++){
        line_ptr = (simple_str **)dynamic_array_get_element(file_ptr -> lines , i);
        len = simple_str_get_strlen(*line_ptr);
        tmp = simple_str_get_string(*line_ptr);
        tmp[len] = '\n';
        

        fwrite(tmp , sizeof(char) , len + 1 , target_file);
    
        free(line_ptr);
        free(tmp);
    } 

    line_ptr = (simple_str **)dynamic_array_get_element(file_ptr -> lines , simple_file_get_line_no(file_ptr) - 1);

    len = simple_str_get_strlen(*line_ptr);
    tmp = simple_str_get_string(*line_ptr);
    
    fwrite(tmp , sizeof(char) , len , target_file);

    free(line_ptr);
    free(tmp);

    fclose(target_file);

    file_ptr -> changes_saved = true;
}

void destroy_simple_file(simple_file *file_ptr){
    if(file_ptr == NULL) return ;

    destroy_dynamic_array(file_ptr -> lines);
    destroy_linked_list(file_ptr -> changes_stack);
    destroy_linked_list(file_ptr -> undone_stack);

    free(file_ptr -> file_name);
    free(file_ptr);
}

size_t simple_file_get_line_no(simple_file *file_ptr){
    if(file_ptr == NULL) return 0;

    return dynamic_array_get_elements_no(file_ptr -> lines);
}

dynamic_array *simple_file_get_content(simple_file *file_ptr){
    if(file_ptr == NULL) return NULL;

    return file_ptr -> lines;
}

char *simple_file_get_line(simple_file *file_ptr , size_t index){
    if(file_ptr == NULL) return NULL;
    if(index >= simple_file_get_line_no(file_ptr)) return NULL;

    simple_str **str = (simple_str **)dynamic_array_get_element(file_ptr -> lines , index);
    char *ret = simple_str_get_string(*str);

    free(str);

    return ret;
}

void simple_file_copy_line(simple_file *file_ptr , size_t index , size_t start_pos , char *dest , size_t dest_size){
    if(file_ptr == NULL || dest == NULL || dest_size == 0) return ;
    if(index >= simple_file_get_line_no(file_ptr)) return ;

    simple_str **str = (simple_str **)dynamic_array_get_element(file_ptr -> lines , index);
    simple_str_copy_str(*str , start_pos , dest , dest_size);

    free(str);
}

size_t simple_file_get_line_len(simple_file *file_ptr , size_t index){
    if(file_ptr == NULL) return 0;
    if(index >= simple_file_get_line_no(file_ptr)) return 0;

    simple_str **str = (simple_str **)dynamic_array_get_element(file_ptr -> lines , index);
    size_t ret = simple_str_get_strlen(*str);

    free(str);

    return ret;
}

bool simple_file_changes_saved(simple_file *file_ptr){
    if(file_ptr == NULL) return false;

    return file_ptr -> changes_saved;
}

const char *simple_file_get_name(simple_file *file_ptr){
    if(file_ptr == NULL) return NULL;

    return file_ptr -> file_name;
}