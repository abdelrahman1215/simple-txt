#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"

#include <pdcurses.h>
#include <stdlib.h>

void normal_mode(int input , WINDOW *inp_window){
    if(input == ERR) return ; 

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

        case 'i':
        case 'I':
            Current_Mode = Insert_Mode;
            break;

        case 'd':
        case 'D':
            simple_file_delete_lines(Current_File , simple_file_get_curr_line(Current_File) , 1);
            break;

        case 'q':
        case 'Q':
            exit(0);
    }
}