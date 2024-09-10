#include "../headers/simple_globals.h"
#include "../headers/simple_file.h"

#include <pdcurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int Cursor_X = -1 , Cursor_Y = -1;
bool display_started = false;


void move_ncols_right(size_t cols){
    if(display_started == false) return ;
    if(cols == 0) return ;
    if(Col_Pos + cols > simple_file_get_line_len(Current_File , Line_Pos)) return ;

    unsigned int disp_width = Txt_Disp_End_X - Txt_Disp_Start_X;

    Col_Pos += cols;

    if(Col_Pos - Txt_Start_Col >= disp_width - Least_H_Distance){
        Txt_Start_Col = Col_Pos - (disp_width - Least_H_Distance);
    }

    Cursor_X = (Col_Pos - Txt_Start_Col) + Txt_Disp_Start_X;
}

void move_ncols_left(size_t cols){
    if(display_started == false) return ;
    if(cols == 0) return ;
    if(cols > Col_Pos) return ;

    Col_Pos -= cols;

    if(Col_Pos <= Txt_Start_Col + Least_H_Distance && Txt_Start_Col > 0){
        if(Col_Pos < Least_H_Distance){
            Txt_Start_Col = 0;
        }else{
            Txt_Start_Col = Col_Pos - Least_H_Distance;
        }
    }

    Cursor_X = (Col_Pos - Txt_Start_Col) + Txt_Disp_Start_X;
}

void move_nlines_down(size_t lines){
    if(display_started == false) return ;
    if(lines == 0) return ;
    if(Line_Pos + lines >= simple_file_get_line_no(Current_File)) return ;

    unsigned int disp_height = Txt_Disp_End_Y - Txt_Disp_Start_Y;

    Line_Pos += lines;

    if(Line_Pos - Txt_Start_Line >= disp_height - Least_V_Distance){
        Txt_Start_Line = Line_Pos - (disp_height - Least_V_Distance) + 1;
    }

    Cursor_Y = Txt_Disp_Start_Y + (Line_Pos - Txt_Start_Line);

    size_t line_len = simple_file_get_line_len(Current_File , Line_Pos);
    if(Col_Pos > line_len){
        Col_Pos = line_len;

        unsigned int disp_width = Txt_Disp_End_X - Txt_Disp_Start_X;
        if(Txt_Start_Col > 0 && Txt_Start_Col + Least_H_Distance > line_len){
            if(Col_Pos > disp_width - Least_H_Distance){
                Txt_Start_Col = line_len - (disp_width - Least_H_Distance);
            }else{
                Txt_Start_Col = 0;
            }
        }

        Cursor_X = Txt_Disp_Start_X + (line_len - Txt_Start_Col);
    }
}

void move_nlines_up(size_t lines){
    if(display_started == false) return ;
    if(lines == 0) return ;
    if(Line_Pos == 0) return ;

    Line_Pos -= lines;

    if(Line_Pos <= Txt_Start_Line + Least_V_Distance && Txt_Start_Line > 0){
        if(Line_Pos < Least_V_Distance){
            Txt_Start_Line = 0;
        }else{
            Txt_Start_Line = Line_Pos - Least_V_Distance;
        }
    }
    
    Cursor_Y = Txt_Disp_Start_Y + (Line_Pos - Txt_Start_Line);

    size_t line_len = simple_file_get_line_len(Current_File , Line_Pos);
    if(Col_Pos > line_len){
        Col_Pos = line_len;

        unsigned int disp_width = Txt_Disp_End_X - Txt_Disp_Start_X;
        if(Txt_Start_Col > 0 && Txt_Start_Col + Least_H_Distance > line_len){
            if(Col_Pos > disp_width - Least_H_Distance){
                Txt_Start_Col = line_len - (disp_width - Least_H_Distance);
            }else{
                Txt_Start_Col = 0;
            }
        }

        Cursor_X = Txt_Disp_Start_X + (line_len - Txt_Start_Col);
    }
}

void move_to_pos(size_t line_index , size_t col_index){
    if(display_started == false) return ;
    if(line_index == Line_Pos && col_index == Col_Pos) return ;

    if(line_index >= simple_file_get_line_no(Current_File)) return ;
    if(col_index > simple_file_get_line_len(Current_File , line_index)) return ;

    if(line_index != Line_Pos){
        if(line_index > Line_Pos){
            move_nlines_down(line_index - Line_Pos);
        }else{
            move_nlines_up(Line_Pos - line_index);
        }
    }

    if(col_index != Col_Pos){
        if(col_index > Col_Pos){
            move_ncols_right(col_index - Col_Pos);
        }else{
            move_ncols_left(Col_Pos - col_index);
        }
    }
}

void update_text_display(){
    if(stdscr == NULL) return ;

    if(Current_File == NULL) return ;

    unsigned int row_no = Txt_Disp_End_Y - Txt_Disp_Start_Y;
    unsigned int max_len = Txt_Disp_End_X - Txt_Disp_Start_X;
    
    char rows[row_no][max_len + 1];
    for(unsigned int i = 0 ; i < row_no ; i++){
        memset(rows[i] , ' ' , max_len + 1);
        rows[i][max_len] = '\000';
    }

    char *tmp = NULL;
    unsigned int used_rows = 0;
    size_t line_len;
    for(unsigned int i = 0 ; i < row_no ; i++ , used_rows++){
        tmp = simple_file_get_line(Current_File , i + Txt_Start_Line);
        if(tmp == NULL) break;
        line_len = simple_file_get_line_len(Current_File , i + Txt_Start_Line);
        if(Txt_Start_Col >= line_len) continue;

        strncpy(rows[i] , tmp + Txt_Start_Col , max_len > (line_len - Txt_Start_Col) ? (line_len - Txt_Start_Col) : max_len);

        free(tmp);
    }

    for(unsigned int i = used_rows ; i < row_no ; i++){
        rows[i][0] = '~';
    }

    for(unsigned int i = 0 ; i < row_no ; i++){
        mvprintw(Txt_Disp_Start_Y + i , Txt_Disp_Start_X , "%s" , rows[i]);
    }

    if(Cursor_X == -1 && Cursor_Y == -1){
        Cursor_X = Txt_Disp_Start_X;
        Cursor_Y = Txt_Disp_Start_Y;
    }

    move(Cursor_Y , Cursor_X);

    display_started = true;
}