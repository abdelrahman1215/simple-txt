#include "../c_datastructures/headers/linked_list.h"
#include "../headers/simple_globals.h"
#include "../headers/curses_header.h"
#include "../headers/simple_file.h"
#include "../headers/simple_str.h"
#include "../headers/display.h"
#include "../headers/get_str.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct input_history{
    linked_list *input_list;
    size_t history_size;
    size_t history_max;
};

input_history *new_input_history(size_t max_history_size){
    if(max_history_size == 0) return NULL;

    input_history *ret = calloc(1 , sizeof(input_history));
    if(ret == NULL) return NULL;

    ret -> input_list = new_linked_list();
    if(ret -> input_list == NULL){
        free(ret);
        
        return NULL;
    }

    ret -> history_size = 0;
    ret -> history_max = max_history_size;

    return ret;
}

void destroy_input_history(input_history *history_ptr){
    if(history_ptr == NULL) return ;

    destroy_linked_list(history_ptr -> input_list);
    free(history_ptr);
}

char *get_str(WINDOW *inp_window , input_history *history_ptr , autocomp_func autocomplete , unsigned int background_pair , unsigned int text_pair , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned int end_y){
    if(inp_window == NULL) return NULL;

    if(start_x == 0 && end_x == 0){
        end_x = getmaxx(inp_window);
    }

    if(start_y == 0 && end_y == 0){
        end_y = getmaxy(inp_window);
    }

    if((start_x >= end_x && start_x == 0) || (start_y >= end_y && start_y == 0)) return NULL;

    unsigned int win_width = getmaxx(inp_window) , win_height = getmaxy(inp_window);
    if(end_x > win_width || end_y > win_height) return NULL;

    loading_err error;
    simple_file *buffer = load_from_str("" , &error);
    if(buffer == NULL){
        return NULL;
    }

    char tmp[2] = " ";

    size_t col_pos = 0 , line_len = 0;

    text_display_info *save_info = new_text_disp_info();
    if(save_info == NULL) return NULL;

    bool cycle_thru_opts = false;
    bool get_history = false;
    size_t opt_index = 0;
    autocomp_info info = {.rep_options = NULL};

    update_text_display(buffer , save_info , inp_window , background_pair , text_pair , 0 , 0 , 0 , 0 , 0 , false , false , false , false , true , start_x , end_x , start_y , end_y);
    wrefresh(inp_window);

    size_t input_index = 0;
    if(history_ptr != NULL){
        input_index = history_ptr -> history_size;//index of last element + 1 bec the current input is going to be the last element
    }

    simple_file *temp_file = NULL;

    for(int ch = wgetch(inp_window) ; ch != '\n' ; ch = wgetch(inp_window)){
        if(ch == ERR) continue;

        col_pos = simple_file_get_curr_column(buffer);
        //line_len = simple_file_get_line_len(buffer , 0);
        
        if(ch == '\t'){
            if(autocomplete == NULL) continue;

            char *input = simple_file_get_line(buffer , 0);
            if(input == NULL) return NULL;

            autocomp_info tmp_info = autocomplete(input , col_pos , !cycle_thru_opts);
            info.replace_start = tmp_info.replace_start;
            info.replace_end = tmp_info.replace_end;
            if(!cycle_thru_opts){
                if(info.rep_options != NULL){
                    for(size_t i = 0 ; i < info.opt_no ; i++){
                        free(info.rep_options[i]);
                    }

                    free(info.rep_options);
                }

                info.opt_no = tmp_info.opt_no;
                info.rep_options = tmp_info.rep_options;
            }

            free(input);
            if(info.rep_options == NULL) continue;

            size_t del_count = info.replace_end - info.replace_start;

            simple_file_delete(buffer , 0 , info.replace_start , del_count);
            simple_file_add(buffer , 0 , info.replace_start , info.rep_options[opt_index]);

            if(info.opt_no > 1){
                cycle_thru_opts = true;
                opt_index ++;
            }

            if(opt_index >= info.opt_no){
                opt_index = 0;
            }

            update_text_display(buffer , save_info , inp_window , background_pair , text_pair , 0 , 0 , 0 , 0 , 0 , false ,  false , false , false , true , start_x , end_x , start_y , end_y);
            wrefresh(inp_window);
            continue;
        }

        cycle_thru_opts = false;
        opt_index = 0;

        switch(ch){
            case '\e':
                free(save_info);
                return NULL;

            case KEY_LEFT:
                simple_file_move_ncols_left(buffer , 1);

                break;

            case KEY_RIGHT:
                simple_file_move_ncols_right(buffer , 1);

                break;

            case KEY_UP:
                if(history_ptr == NULL || input_index == 0) break;

                input_index--;
                get_history = true;

                break;

            case KEY_DOWN:
                if(history_ptr == NULL) break;
                if(input_index == history_ptr -> history_size) break;

                input_index++;
                get_history = true;

                break;

            case KEY_BACKSPACE:
            case 127:
            case '\b':
                if(col_pos > 0){
                    simple_file_delete(buffer , 0 , col_pos - 1 , 1);
                }else{
                    destroy_simple_file(buffer);
                    free(save_info);

                    return NULL;
                }

                break;

            case KEY_DC:
                simple_file_delete(buffer , 0 , col_pos , 1);

                break;
            
            default :{
                char tmp_buff[2] = {'\000' , '\000'};

                for(int tmp = ch ; tmp != ERR ; tmp = wgetch(inp_window)){
                    if(!isprint(tmp) && tmp != '\t') break ;

                    tmp_buff[0] = tmp;
                    if(tmp == '\t'){
                        tmp_buff[0] = ' ';
                    }

                    simple_file_add(buffer , 0 , simple_file_get_line_len(buffer , 0) , tmp_buff);
                }

                break;
            }
        }

        if(get_history && history_ptr != NULL/*this operand is here just to silence clang-tidy warnings*/){
            if(history_ptr -> history_size == 0) goto display;

            if(input_index < history_ptr -> history_size){
                node *target_node = linked_list_get_node(input_index , history_ptr -> input_list);
                char *text = linked_list_get_obj(target_node);
                if(text == NULL)  goto display;

                if(temp_file == NULL){
                    temp_file = buffer;
                    loading_err error;
                    buffer = load_from_str(text , &error);
                    simple_file_move_ncols_right(buffer , simple_file_get_line_len(buffer , 0));
                }else{
                    simple_file_delete(buffer , 0 , 0 , simple_file_get_line_len(buffer , 0));
                    simple_file_add(buffer , 0 , 0 , text);
                }

                free(text);
            }else if(temp_file != NULL && input_index == history_ptr -> history_size){
                destroy_simple_file(buffer);

                buffer = temp_file;
                temp_file = NULL;
            }

            get_history = false;
        }

        display : {
            update_text_display(buffer , save_info , inp_window , background_pair , text_pair , 0 , 0 , 0 , 0 , 3 , false , false , false , false , true , start_x , end_x , start_y , end_y);
            wrefresh(inp_window);
        }
    }

    if(info.rep_options != NULL){
        for(size_t i = 0 ; i < info.opt_no ; i++){
            free(info.rep_options[i]);
        }

        free(info.rep_options);
    }

    free(save_info);

    simple_str *ret_buff = new_simple_str("");
    if(ret_buff == NULL){
        destroy_simple_file(buffer);

        return NULL;
    }

    size_t line_no = simple_file_get_line_no(buffer);
    for(size_t i = 0 ; i < line_no ; i++){
        char *tmp = simple_file_get_line(buffer , i);
        simple_str_add(ret_buff , tmp , simple_str_get_strlen(ret_buff));
        free((char *)tmp);
    }

    char *ret = simple_str_get_string(ret_buff);

    destroy_simple_str(ret_buff);
    destroy_simple_file(buffer);

    if(history_ptr != NULL){
        linked_list_add_node((void *)ret , strlen(ret) + 1 , NULL , history_ptr -> input_list);
        (history_ptr -> history_size)++;

        if(history_ptr -> history_size > history_ptr -> history_max){
            linked_list_delete_node(0 , history_ptr -> input_list);\
            (history_ptr -> history_size)--;
        }
    }

    return ret;
}