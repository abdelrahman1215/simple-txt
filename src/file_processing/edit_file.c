#include "../../headers/simple_file.h"
#include "../../headers/simple_str.h"
#include "../../headers/div_str.h"
#include "simple_file_struct.c"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void simple_file_add_empty_lines(simple_file *file_ptr , size_t line_no , size_t line_index){
    if(file_ptr == NULL) return ;
    if(line_index > simple_file_get_line_no(file_ptr) - 1) return ;

    dynamic_array *contents = file_ptr -> lines;
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

    dynamic_array *contents = file_ptr -> lines;

    simple_str **target = (simple_str **)dynamic_array_get_element(contents , line_index);
    size_t len = simple_str_get_strlen(*target);
    if(pos_index > len){
        free(target);

        return;
    }

    size_t line_no;
    char **divided = div_str_into_lines(source , &line_no);
    
    if(line_no == 0){
        free(target);

        return;
    }

    simple_str_add(*target , divided[0] , pos_index);

    free(target);
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

void simple_file_delete(simple_file *file_ptr , size_t line_index , size_t start_pos , size_t count){
    if(file_ptr == NULL) return ;
    if(line_index > simple_file_get_line_no(file_ptr) - 1 || count == 0) return ;

    dynamic_array *content = file_ptr -> lines;
    simple_str **target_line_ptr = (simple_str **)dynamic_array_get_element(content , line_index);

    size_t len = simple_str_get_strlen(*target_line_ptr);
    if(start_pos + count > len){
        free(target_line_ptr);

        return ;
    }

    simple_str_delete(*target_line_ptr , start_pos , count);

    free(target_line_ptr);
}

void simple_file_delete_lines(simple_file *file_ptr , size_t line_index , size_t line_count){
    if(file_ptr == NULL) return ;
    if(line_index + line_count > simple_file_get_line_no(file_ptr)) return ;

    dynamic_array *content = file_ptr ->lines;

    for(size_t i = line_index ; i < line_index + line_count ; i++){
        dynamic_array_remove_element(content , line_index);
    }
}