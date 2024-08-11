#include "../../headers/simple_file.h"
#include "simple_file_struct.c"

#include <stddef.h>
#include <stdlib.h>


#define changes_limit 100

#define addition 0
#define deletion 1

typedef struct file_addition {
    size_t start_line , start_pos , end_line, end_pos;
}file_addition;

typedef struct file_deletion {
    size_t line_index , position;
    char *deleted_text;
} file_deletion;

typedef struct file_change {
    int change_type;
    void *change_info;
} file_change;

void free_change_struct(void *change){
    file_change *change_ptr = (file_change *)change;
    if(change_ptr -> change_type == deletion){
        struct file_deletion *tmp = (struct file_deletion *) change_ptr -> change_info;

        free(tmp -> deleted_text);
    }

    free((file_change *)change_ptr -> change_info);

    free(change_ptr);
}

void push_change(simple_file *file_ptr , file_change change){
    linked_list_add_node(&change , sizeof(file_change) , free_change_struct , file_ptr -> changes_stack);

    if(linked_list_get_node_no(file_ptr -> changes_stack) == changes_limit){
        linked_list_delete_node(0 , file_ptr -> changes_stack);
    }
}

file_change *pull_last_change(simple_file *file_ptr){
    node *tmp = linked_list_get_last_node(file_ptr -> changes_stack);

    file_change *ret = (file_change *)linked_list_get_obj(tmp);
    return ret;
}

void log_addition(simple_file *file_ptr , size_t start_line , size_t start_pos , size_t end_line , size_t end_pos){
    if(start_line > end_line  || (start_line == end_line && start_pos >= end_pos)) return ;
    if(end_line >= simple_file_get_line_no(file_ptr)) return;

    file_addition *add_ptr = malloc(sizeof(file_addition));
    if(add_ptr == NULL) return ;

    add_ptr -> start_line = start_line;
    add_ptr -> start_pos = start_pos;
    add_ptr -> end_line = end_line;
    add_ptr -> end_pos = end_pos;

    file_change change = {.change_type = addition , .change_info = (void *)add_ptr};
    push_change(file_ptr , change);
}

void log_deletion(simple_file *file_ptr , size_t line_index , size_t position , char *deleted_text){
    if(deleted_text == NULL || line_index >= simple_file_get_line_no(file_ptr)) return ;

    file_deletion *del_ptr = malloc(sizeof(file_deletion));
    if(del_ptr == NULL) return ;

    del_ptr -> deleted_text = calloc(strlen(deleted_text) + 1 , sizeof(char));
    if(del_ptr -> deleted_text == NULL){
        free(del_ptr);
        return;
    }

    del_ptr -> line_index = line_index;
    del_ptr -> position = position;

    file_change change = {.change_type = deletion , .change_info = (void *)del_ptr};
    push_change(file_ptr , change);
}