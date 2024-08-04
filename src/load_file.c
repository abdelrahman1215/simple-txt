#include "../c_datastructures/headers/dynamic_array.h"
#include "../headers/simple_str.h"
#include "../headers/load_file.h"

#include <sys/stat.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct file_struct{
    char *file_name;

    dynamic_array *lines;
    size_t line_no;
};

bool is_dir(const char *file_name){
    struct stat file_stat;

    stat(file_name , &file_stat);

    return (bool) S_ISDIR(file_stat.st_mode);
}

#define buff_size 20//(8 * 1024 * 1024) - 1

void free_simple_str(simple_str **str_ptr_ptr){
    destroy_simple_str(*str_ptr_ptr);
    free(str_ptr_ptr);
}

simple_file *load_file(const char *file_name , loading_err *get_err){
    *get_err = OK;

    if(get_err == NULL) return NULL;
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

    FILE *target_file = fopen(file_name , "r");
    if(target_file == NULL){
        *get_err = File_Not_Found;

        return NULL;
    }

    simple_file *ret = malloc(sizeof(simple_file));
    if(ret == NULL){
        *get_err = Alloc_Err;

        return NULL;
    }

    size_t len = strnlen(file_name , 9223372036854775807);
    ret -> file_name = calloc(len + 1 , sizeof(char));
    if(ret -> file_name == NULL){
        free(ret);

        *get_err = Alloc_Err;
        return NULL;
    }

    strncpy(ret -> file_name , file_name , len);

    ret -> lines = new_dynamic_array(sizeof(simple_str *) , (free_func *)free_simple_str);
    if(ret -> lines == NULL){
        free(ret -> file_name);
        free(ret);

        *get_err = Alloc_Err;
        return NULL;
    }

    ret -> line_no = 0;

    char *buff = calloc(buff_size + 1 , sizeof(char));
    if(buff == NULL){
        destroy_dynamic_array(ret -> lines);
        free(ret -> file_name);
        free(ret);

        *get_err = Alloc_Err;
        return NULL;
    }

    simple_str *file_buff = new_simple_str(NULL);
    if(file_buff == NULL){
        free(buff);

        destroy_dynamic_array(ret -> lines);
        free(ret -> file_name);
        free(ret);

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

    char *file_text = simple_str_get_string(file_buff);
    if(file_text == NULL){
        free(file_buff);

        destroy_dynamic_array(ret -> lines);
        free(ret -> file_name);
        free(ret);

        *get_err = Alloc_Err;
        return NULL;
    }

    destroy_simple_str(file_buff);

    simple_str *line = NULL;

    size_t line_start = 0;
    for(size_t i = 0 ; file_text[i] != '\000' ; i++){
        if(file_text[i] == '\n'){
            file_text[i] = '\000';

            line = new_simple_str(file_text + line_start);

            dynamic_array_add_element(ret -> lines , &line);

            line_start = i + 1;

            ret -> line_no ++;
        }
    }


    line = new_simple_str(file_text + line_start);
    dynamic_array_add_element(ret -> lines , &line);    
    ret -> line_no ++;

    if(total_read_bytes > 0){
        free(file_text);
    }
    fclose(target_file);
    return ret;
}

void destroy_file(simple_file *file_ptr){
    destroy_dynamic_array(file_ptr -> lines);
    free(file_ptr -> file_name);
    free(file_ptr);
}

size_t simple_file_get_line_no(simple_file *file_ptr){
    if(file_ptr == NULL) return 0;

    return file_ptr -> line_no;
}

dynamic_array *simple_file_get_content(simple_file *file_ptr){
    if(file_ptr == NULL) return NULL;

    return file_ptr -> lines;
}

char *simple_file_get_line(simple_file *file_ptr , size_t index){
    if(file_ptr == NULL) return NULL;
    if(index > file_ptr -> line_no - 1) return NULL;

    simple_str **str = (simple_str **)dynamic_array_get_element(file_ptr -> lines , index);
    char *ret = simple_str_get_string(*str);

    return ret;
}