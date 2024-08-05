#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "../headers/load_file.h"
#include "../headers/simple_str.h"

char **div_str_into_lines(char *str , size_t *get_line_no){
    if(str == NULL || get_line_no == NULL) return NULL;

    *get_line_no = 1;
    size_t len = strlen(str);
    char *copy = calloc(len + 1 , sizeof(char));
    strncpy(copy , str , len);

    for(size_t i = 0 ; i < len ; i++){
        if(copy[i] == '\n'){
            copy[i] = '\000';
            (*get_line_no) ++;
        }
    }

    char **ret = malloc(sizeof(char *) * (*get_line_no));
    if(ret == NULL){
        free(copy);
        *get_line_no = 0;
        
        return NULL;
    }

    size_t line_start = 0 , lines = 0;

    for(size_t i = 0 ; i < len ; i++){
        if(copy[i] == '\000'){
            ret[lines ++] = copy + line_start;
            line_start = i + 1;
        }
    }

    ret[lines] = copy + line_start;

    return ret;
}

void simple_file_add_empty_lines(simple_file *file_ptr , size_t line_no , size_t line_index){
    if(file_ptr == NULL) return ;
    if(line_index > simple_file_get_line_no(file_ptr) - 1) return ;

    dynamic_array *contents = simple_file_get_content(file_ptr);
    size_t initial_line_no = simple_file_get_line_no(file_ptr);


    simple_str *new_line;
    for(size_t i = 0 ; i < line_no ; i++){
        new_line = new_simple_str(NULL);
        dynamic_array_add_element(contents , &new_line);
    }

    simple_str **line1_ptr;
    simple_str **line2_ptr;
    char *tmp;
    for(size_t i = initial_line_no - 1 ; i >= line_index ; i--){
        line1_ptr = (simple_str **)dynamic_array_get_element(contents , i);
        line2_ptr = (simple_str **)dynamic_array_get_element(contents , i + line_no);
        tmp = simple_str_get_string(*line1_ptr);

        simple_str_delete(*line1_ptr , 0 , simple_str_get_strlen(*line1_ptr));
        simple_str_delete(*line2_ptr , 0 , simple_str_get_strlen(*line2_ptr));

        simple_str_add(*line2_ptr , tmp , 0);

        free(line1_ptr);
        free(line2_ptr);
        free(tmp);
    }
}

void simple_file_add(simple_file *file_ptr , size_t line_index , size_t pos_index , char *source){
    if(file_ptr == NULL) return ;
    if(line_index > simple_file_get_line_no(file_ptr) - 1) return ;
    if(source == NULL) return ;
    if(*source == '\000') return ;

    dynamic_array *contents = simple_file_get_content(file_ptr);

    simple_str **target = (simple_str **)dynamic_array_get_element(contents , line_index);
    size_t len = simple_str_get_strlen(*target);
    if(pos_index > len) return ;

    size_t line_no;
    char **divided = div_str_into_lines(source , &line_no);
    
    if(line_no == 0) return ;

    simple_str_add(*target , divided[0] , pos_index);
    if(line_no > 0){
        simple_file_add_empty_lines(file_ptr , line_no - 1 , line_index + 1);

        for(size_t i = 1 ; i < line_no ; i++){
            target = (simple_str **)dynamic_array_get_element(contents , line_index + i);
            simple_str_add(*target , divided[i] , 0);

            free(target);
        }
    } 

    free(divided[0]);
    free(divided);
}