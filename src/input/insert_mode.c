#include "../../headers/simple_globals.h"
#include "../../headers/curses_header.h"
#include "../../headers/simple_file.h"

#include <string.h>
#include <ctype.h>

void insert_mode(int input , WINDOW *inp_window){
    if(input == ERR) return ;

    size_t line_pos = simple_file_get_curr_line(Current_File);
    size_t col_pos = simple_file_get_curr_column(Current_File);
    size_t line_no = simple_file_get_line_no(Current_File);

    size_t line_len = simple_file_get_line_len(Current_File , line_pos);

    switch(input){
        case '\e'://escape
            Current_Mode = Normal_Mode;
            return;

        case KEY_RIGHT : 
            simple_file_move_ncols_right(Current_File , 1);
            break;

        case KEY_LEFT : 
            simple_file_move_ncols_left(Current_File , 1);
            break;
        
        case KEY_DOWN : 
            simple_file_move_nlines_down(Current_File , 1);
            break;

        case KEY_UP : 
            simple_file_move_nlines_up(Current_File , 1);
            break;

        case CTRL('z'):
            simple_file_undo(Current_File);
            break;

        case CTRL('y'):
            simple_file_redo(Current_File);
            break;

        case CTRL('d'):
            simple_file_delete_lines(Current_File , line_pos , 1);
            break;

        case '\b'://backspace
            if(col_pos > 0){
                simple_file_delete(Current_File , line_pos , col_pos - 1 , 1);
            }else if(line_pos > 0){
                size_t prev_line_len = simple_file_get_line_len(Current_File , line_pos - 1);
                simple_file_delete_from_to(Current_File , line_pos - 1 , prev_line_len , line_pos , 0);
            }

            break;
            
        case KEY_DC:
            if(col_pos < line_len){
                simple_file_delete(Current_File , line_pos , col_pos , 1);
            }else if(line_pos + 1 < line_no){
                simple_file_delete_from_to(Current_File , line_pos , line_len , line_pos + 1 , 0);
            }

            break;
        
        default:
            char tmp[2] = {'\000' , '\000'};

            char tab[Tab_Size + 1];
            memset(tab , ' ' , Tab_Size);
            tab[Tab_Size] = '\000';
            
            for(int ch = input ; ch != ERR && (isprint(ch) || ch == '\n' || ch == '\t') ; ch = wgetch(inp_window)){
                line_pos = simple_file_get_curr_line(Current_File);
                col_pos = simple_file_get_curr_column(Current_File);

                if(ch == '\t'){
                    simple_file_add(Current_File , line_pos , col_pos , tab);
                }else{
                    tmp[0] = ch;
                    simple_file_add(Current_File , line_pos , col_pos , tmp);
                }
            }

            break;
    } 
}