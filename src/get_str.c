#include "../headers/simple_globals.h"
#include "../headers/simple_file.h"
#include "../headers/simple_str.h"
#include "../headers/display.h"
#include "../headers/get_str.h"

#include <pdcurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>


char *get_str(WINDOW *inp_window , unsigned int background_pair , unsigned int text_pair , bool warp_text , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned int end_y){
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

    size_t line_pos = 0; 
    size_t col_pos = 0;

    text_display_info *save_info = new_text_disp_info();
    if(save_info == NULL) return NULL;

    for(int ch = wgetch(inp_window) ; ch != '\n' ; ch = wgetch(inp_window)){
        line_pos = simple_file_get_curr_line(buffer);
        col_pos = simple_file_get_curr_column(buffer);

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

            case '\b':
                if(col_pos > 0){
                    simple_file_delete(buffer , line_pos , col_pos - 1 , 1);
                }

                break;

            case KEY_DC:
                simple_file_delete(buffer , line_pos , col_pos , 1);

                break;


            default :
                for(int tmp = ch ; tmp != ERR ; tmp = wgetch(inp_window)){
                    if(!isprint(tmp)) continue ;

                    if(col_pos == win_width || warp_text){
                        simple_file_add(buffer , line_pos , col_pos , "\n");
                    }

                    char tmp_buff[2] = {(char)tmp , '\000'};
                    simple_file_add(buffer , line_pos , col_pos , tmp_buff);
                }
        }

        update_text_display(buffer , save_info , inp_window , background_pair , text_pair , 0 , 0 , 0 , 0 , 0 , false , false , false , false , start_x , end_x , start_y , end_y);
        wrefresh(inp_window);
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

    return ret;
}