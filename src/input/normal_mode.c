#include "../../headers/simple_globals.h"
#include "../../headers/curses_header.h"
#include "../../headers/simple_file.h"
#include "../../headers/simple_str.h"
#include "../../headers/msg_fmt.h"
#include "../../headers/display.h"
#include "command_mode.c"
#include "binding_map.c"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void move_n_cols_or_lines(int first_char){
    simple_str *buffer;

    char add[2] = {(char) first_char , '\000'};
    buffer = new_simple_str(add);

    if(buffer == NULL) return ;

    WINDOW *disp_win = Lower_Strip_Window;//didn't directly use Lower_Strip_Window in case i wanted to change it later
    int ch;
    char *tmp = simple_str_get_string(buffer);
    display_recorded_binding(disp_win , tmp);
    for(ch = wgetch(disp_win) ; ; ch = wgetch(disp_win)){
        if(ch == ERR) continue;

        add[0] = ch;
        simple_str_add(buffer , add , simple_str_get_strlen(buffer));

        free(tmp);
        tmp = simple_str_get_string(buffer);

        display_recorded_binding(disp_win , tmp);
        update_panels();
        doupdate();

        if(!isdigit(ch)) break;
    }

    size_t moves_no = atoi(tmp);
    free(tmp);

    size_t curr_line = simple_file_get_curr_line(Current_File) , curr_col = simple_file_get_curr_column(Current_File);
    size_t line_no = simple_file_get_line_no(Current_File) , line_len = simple_file_get_line_len(Current_File , curr_line);
    switch(ch){//will be equal to the last character returned by wgetch
        case 'j':
            if(moves_no <= curr_line) simple_file_move_nlines_up(Current_File , moves_no);
            usleep(100000);
            
            break;

        case 'k':
            if(curr_line + moves_no < line_no) simple_file_move_nlines_down(Current_File , moves_no);
            usleep(100000);
            
            break;

        case 'h':
            if(moves_no <= curr_col) simple_file_move_ncols_left(Current_File , moves_no);
            usleep(100000);
            
            break;

        case 'l':
            if(curr_col + moves_no <= line_len) simple_file_move_ncols_right(Current_File , moves_no);
            usleep(100000);
            
            break;

        case '\r':
        case '\n':
        case '\b':
        case '\e':
            break;

        default :
            custom_msg(Error , "Unkown binding");
    }

    destroy_simple_str(buffer);
}

void normal_mode(int input , WINDOW *inp_window){
    if(input == ERR) return ;

    size_t curr_line = simple_file_get_curr_line(Current_File);

    switch(input){
        case 'l':
        case 'L':
        case KEY_RIGHT : 
            simple_file_move_ncols_right(Current_File , 1);
            break;

        case 'h':
        case 'H':
        case KEY_LEFT : 
            simple_file_move_ncols_left(Current_File , 1);
            break;
        
        case 'k':
        case 'K':
        case KEY_DOWN : 
            simple_file_move_nlines_down(Current_File , 1);
            break;

        case 'j':
        case 'J':
        case KEY_UP : 
            simple_file_move_nlines_up(Current_File , 1);
            break;

        #ifdef LINUX
        case 544 ://alt + left
        #else
        case ALT_LEFT :
        #endif
            simple_file_move_to_xy(Current_File , curr_line , 0);

            break;

        #ifdef LINUX
        case 559 ://alt + right
        #else
        case ALT_RIGHT :
        #endif
            simple_file_move_to_xy(Current_File , curr_line , simple_file_get_line_len(Current_File , curr_line));

            break;

        case 'i':
        case 'I':
            Current_Mode = Insert_Mode;
            break;

        case ':':
            command_mode();
            break;

        default :
            if(isdigit(input)) move_n_cols_or_lines(input);
            else exec_binding(input);

            break;
    }
}