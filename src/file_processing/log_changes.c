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

file_change *copy_change(file_change *change_ptr){
    file_change *ret = malloc(sizeof(file_change));
    if(ret == NULL) return NULL;

    ret -> change_type = change_ptr -> change_type;

    void *change_info;
    if(ret -> change_type == addition){
        file_addition *tmp = malloc(sizeof(file_addition));
        file_addition *src = (file_addition *)change_ptr -> change_info;

        tmp -> start_line = src -> start_line;
        tmp -> start_pos = src -> start_pos;
        tmp -> end_line = src -> end_line;
        tmp -> end_pos = src -> end_pos;

        change_info = (void *)tmp;
    }else{
        file_deletion *tmp = malloc(sizeof(file_deletion));
        file_deletion *src = (file_deletion *)change_ptr -> change_info;

        tmp -> line_index = src -> line_index;
        tmp -> position = src -> position;

        size_t len = strlen(src -> deleted_text);
        tmp -> deleted_text = calloc(len + 1 , sizeof(char));
        if(tmp == NULL){
            free(ret);

            return NULL;
        }

        strncpy(tmp -> deleted_text , src -> deleted_text , len);

        change_info = (void *)tmp;
    }

    ret -> change_info = change_info;

    return ret;
}

void push_change(simple_file *file_ptr , file_change change){
    linked_list_add_node(&change , sizeof(file_change) , free_change_struct , file_ptr -> changes_stack);

    if(linked_list_get_node_no(file_ptr -> changes_stack) == changes_limit){
        linked_list_delete_node(0 , file_ptr -> changes_stack);
    }

    file_ptr -> changes_saved = false;
}

file_change *pop_change(simple_file *file_ptr){
    node *tmp = linked_list_get_last_node(file_ptr -> changes_stack);

    file_change *temp = (file_change *)linked_list_get_obj(tmp);
    file_change *ret = copy_change(temp);

    linked_list_delete_node(linked_list_get_node_no(file_ptr -> changes_stack) - 1 , file_ptr -> changes_stack);
    free(temp);

    file_ptr -> changes_saved = (linked_list_get_node_no(file_ptr -> changes_stack) == 0);

    return ret;
}

void push_undone_change(simple_file *file_ptr , file_change change){
    linked_list_add_node(&change , sizeof(file_change) , free_change_struct , file_ptr -> undone_stack);
    file_ptr -> changes_saved = (linked_list_get_node_no(file_ptr -> changes_stack) == 0);
}

file_change *pull_last_undone_change(simple_file *file_ptr){
    node *tmp = linked_list_get_last_node(file_ptr -> undone_stack);

    file_change *temp = (file_change *)linked_list_get_obj(tmp);
    file_change *ret = copy_change(temp);

    linked_list_delete_node(linked_list_get_node_no(file_ptr -> undone_stack) - 1 , file_ptr -> undone_stack);
    free(temp);

    return ret;
}

void clear_undone_stack(simple_file *file_ptr){
    linked_list *tmp = new_linked_list();
    if(tmp == NULL) return ;

    destroy_linked_list(file_ptr -> undone_stack);
    file_ptr -> undone_stack = tmp;
}

void log_addition(simple_file *file_ptr , size_t start_line , size_t start_pos , size_t end_line , size_t end_pos , bool undone){
    if(start_line > end_line  || (start_line == end_line && start_pos >= end_pos)) return ;
    if(end_line >= simple_file_get_line_no(file_ptr)) return;

    file_addition *add_ptr = malloc(sizeof(file_addition));
    if(add_ptr == NULL) return ;

    add_ptr -> start_line = start_line;
    add_ptr -> start_pos = start_pos;
    add_ptr -> end_line = end_line;
    add_ptr -> end_pos = end_pos;

    file_change change = {.change_type = addition , .change_info = (void *)add_ptr};

    if(undone){
        push_undone_change(file_ptr , change);
    }else{
        push_change(file_ptr , change);
    }
}

void log_deletion(simple_file *file_ptr , size_t line_index , size_t position , char *deleted_text , bool undone){
    if(deleted_text == NULL || line_index >= simple_file_get_line_no(file_ptr)) return ;

    file_deletion *del_ptr = malloc(sizeof(file_deletion));
    if(del_ptr == NULL) return ;

    size_t len = strlen(deleted_text);
    del_ptr -> deleted_text = calloc(len + 1 , sizeof(char));
    if(del_ptr -> deleted_text == NULL){
        free(del_ptr);
        return;
    }

    strncpy(del_ptr -> deleted_text , deleted_text , len);

    del_ptr -> line_index = line_index;
    del_ptr -> position = position;

    file_change change = {.change_type = deletion , .change_info = (void *)del_ptr};

    if(undone){
        push_undone_change(file_ptr , change);
    }else{
        push_change(file_ptr , change);
    }
}