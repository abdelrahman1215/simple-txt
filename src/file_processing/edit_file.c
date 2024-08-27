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
    if(line_index > simple_file_get_line_no(file_ptr)) return ;

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

add_end _add_(simple_file *file_ptr , size_t line_index , size_t pos_index , char *source){
    add_end empty_pair = {.end_line = 0 , .end_pos = 0};
    if(file_ptr == NULL) return empty_pair;
    if(line_index >= simple_file_get_line_no(file_ptr)) return empty_pair;
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
        ret.end_line += line_no - 1;

        size_t len = strlen(divided[line_no - 1]);
        ret.end_pos = len;
    }else{
        ret.end_pos = pos_index + strlen(divided[0]);
    }

    size_t init_len = simple_str_get_strlen(*target);
    bool dd = pos_index < init_len;

    simple_str_add(*target , divided[0] , pos_index);

    if(line_no > 1){
        char *text;
        text = simple_str_get_string(*target);
        size_t added_lines_no = 0;

        added_lines_no = line_no - 1;
        simple_file_add_empty_lines(file_ptr , added_lines_no , line_index + 1);

        if(dd){
            simple_str_delete(*target , pos_index + strlen(divided[0]) , init_len - pos_index);
            free(target);

            target = (simple_str **)dynamic_array_get_element(contents , line_index + added_lines_no);
            simple_str_add(*target , text + pos_index + strlen(divided[0]) , 0);
            free(target);
            free(text);
        }

        for(size_t i = 1 ; i < line_no ; i++){
            target = (simple_str **)dynamic_array_get_element(contents , line_index + i);
            init_len = simple_str_get_strlen(*target);

            simple_str_add(*target , divided[i] , 0);
            text = simple_str_get_string(*target);


            if(i + 1 < line_no && dd){
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
    }

    free(divided[0]);
    free(divided);

    return ret;
}

void simple_file_add(simple_file *file_ptr , size_t line_index , size_t pos_index , char *source){
    add_end end = _add_(file_ptr , line_index , pos_index , source);
    if(end.end_line == 0 && end.end_pos == 0) return ;

    log_addition(file_ptr , line_index , pos_index , end.end_line , end.end_pos , false);
    clear_undone_stack(file_ptr);
}

char *_delete_(simple_file *file_ptr , size_t line_index , size_t start_pos , size_t count){
    if(file_ptr == NULL) return NULL;
    if(line_index >= simple_file_get_line_no(file_ptr) || count == 0) return NULL;

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
    free(text);

    return ret;
}

void simple_file_delete(simple_file *file_ptr , size_t line_index , size_t start_pos , size_t count){
    char *deleted = _delete_(file_ptr , line_index , start_pos , count);
    if(deleted == NULL) return ;

    log_deletion(file_ptr , line_index , start_pos , deleted , false);

    free(deleted);
    clear_undone_stack(file_ptr);
}

//if a logical error happens in the future look at the else statement

char *_delete_lines_(simple_file *file_ptr , size_t line_index , size_t line_count){
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

char *_delete_from_to_(simple_file *file_ptr , size_t start_line , size_t start_pos , size_t end_line , size_t end_pos){
    if(file_ptr == NULL) return NULL;
    if((start_line == end_line && start_pos >= end_pos) || start_line > end_line) return NULL;
    if(start_line >= simple_file_get_line_no(file_ptr) || end_line >= simple_file_get_line_no(file_ptr)) return NULL;
    if(start_pos > simple_file_get_line_len(file_ptr , start_line) || end_pos > simple_file_get_line_len(file_ptr , end_line)) return NULL;

    char *deleted_text;
    if(start_line == end_line){
        deleted_text = _delete_(file_ptr , start_line , start_pos , end_pos - start_pos);
    }else{
        simple_str *buff = new_simple_str(NULL);
        size_t last_line_len = simple_file_get_line_len(file_ptr , end_line);
        char *tmp;

        tmp = _delete_(file_ptr , start_line , start_pos , simple_file_get_line_len(file_ptr , start_line) - start_pos);
        simple_str_add(buff , tmp , simple_str_get_strlen(buff));
        free(tmp);


        if(end_line - start_line > 1){
            tmp = _delete_lines_(file_ptr , start_line + 1 , end_line - start_line - 1);
            simple_str_add(buff , tmp , simple_str_get_strlen(buff));
            free(tmp);
        }else{
            simple_str_add(buff , "\n" , simple_str_get_strlen(buff));
        }

        //if a logical error happens in the future add + 1 after end_pos
        tmp = _delete_(file_ptr , end_line , 0 , end_pos );
        simple_str_add(buff , tmp , simple_str_get_strlen(buff));
        free(tmp);

        if(end_pos != last_line_len){
            tmp = simple_file_get_line(file_ptr , start_line + 1);
            _add_(file_ptr , start_line , simple_file_get_line_len(file_ptr , start_line) , tmp);
            free(tmp);

            tmp = _delete_lines_(file_ptr , start_line + 1 , 1);
            free(tmp);
        }

        deleted_text = simple_str_get_string(buff);
        destroy_simple_str(buff);
    }

    return deleted_text;
}

void simple_file_delete_from_to(simple_file *file_ptr , size_t start_line , size_t start_pos , size_t end_line , size_t end_pos){
    char *deleted_text = _delete_from_to_(file_ptr  , start_line , start_pos , end_line , end_pos);

    if(deleted_text == NULL) return ;

    log_deletion(file_ptr , start_line , start_pos , deleted_text , false);
    free(deleted_text);
}

void simple_file_delete_lines(simple_file *file_ptr , size_t line_index , size_t line_count){
    char *deleted_lines = _delete_lines_(file_ptr , line_index , line_count);
    if(deleted_lines == NULL) return ;

    size_t position = 0;
    size_t ln_index = 0;
    if(line_index > 0){
        position = simple_file_get_line_len(file_ptr , line_index - 1);

        ln_index = line_index - 1;
    }

    log_deletion(file_ptr , ln_index , position , deleted_lines , false);

    free(deleted_lines);
    clear_undone_stack(file_ptr);
}

char *_undo_addition_(file_addition *addition_info , simple_file *file_ptr){
    if(addition_info -> end_line == 0 && addition_info->end_pos == 0) return NULL;
    char *deleted_text = _delete_from_to_(file_ptr , addition_info -> start_line , addition_info -> start_pos , addition_info -> end_line , addition_info -> end_pos);

    return deleted_text;
}

add_end _undo_deletion_(file_deletion *deletion_info , simple_file *file_ptr){
    add_end end = _add_(file_ptr , deletion_info -> line_index , deletion_info -> position , deletion_info -> deleted_text);

    return end;
}

void simple_file_undo(simple_file *file_ptr){
    if(file_ptr == NULL) return ;
    if(linked_list_get_node_no(file_ptr -> changes_stack) == 0) return ;

    file_change *last_change = pull_last_change(file_ptr);
    if(last_change -> change_type == addition){
        file_addition *tmp = last_change -> change_info;

        char *deleted_text = _undo_addition_(tmp , file_ptr);
        if(deleted_text == NULL){
            free_change_struct(last_change);

            return ;
        }
        
        log_deletion(file_ptr , tmp -> start_line , tmp -> start_pos , deleted_text , true);

        free(deleted_text);
    }

    else if(last_change -> change_type == deletion){
        file_deletion *tmp = last_change -> change_info;

        add_end end = _undo_deletion_(tmp , file_ptr);
        log_addition(file_ptr , tmp -> line_index ,tmp -> position , end.end_line , end.end_pos , true);
    }

    free_change_struct(last_change);
}

void simple_file_redo(simple_file *file_ptr){
    if(file_ptr == NULL) return ;
    if(linked_list_get_node_no(file_ptr -> undone_stack) == 0) return ;

    file_change *last_undone = pull_last_undone_change(file_ptr);
    if(last_undone -> change_type == addition){
        file_addition *tmp = last_undone -> change_info;

        char *deleted_text = _undo_addition_(tmp , file_ptr);
        log_deletion(file_ptr , tmp -> start_line , tmp -> start_pos , deleted_text , false);

        free(deleted_text);
    }

    else if(last_undone -> change_type == deletion){
        file_deletion *tmp = last_undone -> change_info;

        add_end end = _undo_deletion_(tmp , file_ptr);
        log_addition(file_ptr , tmp -> line_index ,tmp -> position , end.end_line , end.end_pos , false);
    }

    free_change_struct(last_undone);
}