#include "../../headers/simple_file.h"
#include "../../headers/simple_str.h"
#include "../../headers/div_str.h"
#include "simple_file_struct.c"
#include "log_changes.c"

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

typedef struct add_end{
    size_t end_line , end_pos;
}add_end;

add_end __simple_file_add__(simple_file *file_ptr , size_t line_index , size_t pos_index , char *source){
    add_end empty_pair = {.end_line = 0 , .end_pos = 0};
    if(file_ptr == NULL) return empty_pair;
    if(line_index > simple_file_get_line_no(file_ptr) - 1) return empty_pair;
    if(source == NULL) return empty_pair;
    if(*source == '\000') return empty_pair;

    add_end ret = {.end_line = line_index , .end_pos = 0};

    dynamic_array *contents = file_ptr -> lines;

    simple_str **target = (simple_str **)dynamic_array_get_element(contents , line_index);
    size_t len = simple_str_get_strlen(*target);
    if(pos_index > len){
        free(target);

        return empty_pair;
    }

    size_t line_no;
    char **divided = div_str_into_lines(source , &line_no);
    
    if(line_no == 0){
        free(target);

        return empty_pair;
    }

    if(line_no > 1){
        size_t len = strlen(divided[line_no - 1]);
        ret.end_line += line_no - (len == 0) - 1;
        if(len == 0) len = strlen(divided[line_no - 2]);
        ret.end_pos = len;
    }else{
        ret.end_pos = pos_index + strlen(divided[line_no - 1]);
    }

    size_t init_len = simple_str_get_strlen(*target);

    char *text;
    simple_str_add(*target , divided[0] , pos_index);
    text = simple_str_get_string(*target);

    if(line_no > 0){
        simple_file_add_empty_lines(file_ptr , line_no - 1 , line_index + 1);

        simple_str_delete(*target , pos_index + strlen(divided[0]) , init_len);
        free(target);

        target = (simple_str **)dynamic_array_get_element(contents , line_index + 1);
        simple_str_add(*target , text + line_index + strlen(divided[0]) , 0);
        free(target);
        free(text);

        for(size_t i = 1 ; i < line_no ; i++){
            target = (simple_str **)dynamic_array_get_element(contents , line_index + i);
            init_len = simple_str_get_strlen(*target);

            simple_str_add(*target , divided[i] , 0);
            text = simple_str_get_string(*target);


            if(i + 1 < line_no){
                simple_str_delete(*target , strlen(divided[0]) , init_len);
                free(target);

                target = (simple_str **)dynamic_array_get_element(contents , line_index + i + 1);
                simple_str_add(*target , text + strlen(divided[i]) , 0);

                free(target);
                free(text);
            }else{
                free(target);
                free(text);
            }
        }
    }else{
        free(target);
        free(text);
    }

    free(divided[0]);
    free(divided);

    return ret;
}

void simple_file_add(simple_file *file_ptr , size_t line_index , size_t pos_index , char *source){
    add_end end = __simple_file_add__(file_ptr , line_index , pos_index , source);
    if(end.end_pos == 0) return ;

    log_addition(file_ptr , line_index , pos_index , end.end_line , end.end_pos);
}

char *__simple_file_delete__(simple_file *file_ptr , size_t line_index , size_t start_pos , size_t count){
    if(file_ptr == NULL) return NULL;
    if(line_index > simple_file_get_line_no(file_ptr) - 1 || count == 0) return NULL;

    dynamic_array *content = file_ptr -> lines;
    simple_str **target_line_ptr = (simple_str **)dynamic_array_get_element(content , line_index);

    char *text = simple_str_get_string(*target_line_ptr);
    if(text == NULL){
        free(target_line_ptr);

        return NULL;
    }
    char *ret = calloc(count + 1 , sizeof(char));
    if(text == NULL){
        free(target_line_ptr);
        free(text);

        return NULL;
    }

    strncpy(ret , text + start_pos , count);

    size_t len = simple_str_get_strlen(*target_line_ptr);
    if(start_pos + count > len){
        free(target_line_ptr);
        free(text);
        free(ret);

        return NULL;
    }

    simple_str_delete(*target_line_ptr , start_pos , count);

    free(target_line_ptr);

    return ret;
}

void simple_file_delete(simple_file *file_ptr , size_t line_index , size_t start_pos , size_t count){
    char *deleted = __simple_file_delete__(file_ptr , line_index , start_pos , count);
    if(deleted == NULL) return ;

    log_deletion(file_ptr , line_index , start_pos , deleted);

    free(deleted);
}

char *__simple_file_delete_lines__(simple_file *file_ptr , size_t line_index , size_t line_count){
    if(file_ptr == NULL) return NULL;
    if(line_index + line_count > simple_file_get_line_no(file_ptr)) return NULL;

    dynamic_array *content = file_ptr ->lines;

    simple_str *deleted_lines = new_simple_str("");
    if(line_index > 0){
        simple_str_add(deleted_lines , "\n" , 0);
    }

    simple_str **line = NULL;
    char *line_text = NULL;
    for(size_t i = 0 ; i < line_count ; i++){
        line = (simple_str **)dynamic_array_get_element(content , line_index);

        line_text = simple_str_get_string(*line);
        if(line_text == NULL){
            free(line);
            destroy_simple_str(deleted_lines);

            return NULL;
        }

        simple_str_add(deleted_lines , line_text , simple_str_get_strlen(deleted_lines));
        simple_str_add(deleted_lines , "\n" , simple_str_get_strlen(deleted_lines));

        free(line_text);
        free(line);

        dynamic_array_remove_element(content , line_index);
    }

    char *ret = simple_str_get_string(deleted_lines);
    destroy_simple_str(deleted_lines);

    return ret;
}

void simple_file_delete_lines(simple_file *file_ptr , size_t line_index , size_t line_count){
    char *deleted_lines = __simple_file_delete_lines__(file_ptr , line_index , line_count);
    if(deleted_lines == NULL) return ;

    size_t position = 0;
    size_t ln_index = 0;
    if(line_index > 0){
        char *tmp = simple_file_get_line(file_ptr , line_index - 1);
        position = strlen(tmp);

        free(tmp);

        ln_index = line_index - 1;
    }

    log_deletion(file_ptr , ln_index , position , deleted_lines);

    free(deleted_lines);
}