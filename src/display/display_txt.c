#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/display.h"

#include <pdcurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

size_t Txt_Start_Line = 0 , Txt_Start_Col = 0;
int Cursor_X = 0 , Cursor_Y = 0;
unsigned int Text_Start_X = 0 , Indent = 0;

void update_values(){
    unsigned int disp_width = Txt_Disp_End_X - Text_Start_X;
    size_t line_len = simple_file_get_line_len(Current_File , Line_Pos);
    size_t line_no = simple_file_get_line_no(Current_File);

    if(Col_Pos >= Txt_Start_Col + disp_width - Least_H_Distance && Txt_Start_Col + disp_width < line_len){
        Txt_Start_Col = Col_Pos - (disp_width - Least_H_Distance);
    }else if(Col_Pos <= Txt_Start_Col + Least_H_Distance && Txt_Start_Col > 0){
        if(Col_Pos < Least_H_Distance){
            Txt_Start_Col = 0;
        }else{
            Txt_Start_Col = Col_Pos - Least_H_Distance;
        }
    }
    
    if(Col_Pos > line_len){
        Col_Pos = line_len;

        unsigned int disp_width = Txt_Disp_End_X - Text_Start_X;
        if(Txt_Start_Col > 0 && Txt_Start_Col + Least_H_Distance > line_len){
            if(Col_Pos > disp_width - Least_H_Distance){
                Txt_Start_Col = line_len - (disp_width - Least_H_Distance);
            }else{
                Txt_Start_Col = 0;
            }
        }

        Cursor_X = Text_Start_X + (line_len - Txt_Start_Col);
    }

    Cursor_X = (Col_Pos - Txt_Start_Col) + Text_Start_X;

    unsigned int disp_height = Txt_Disp_End_Y - Txt_Disp_Start_Y;

    if(Line_Pos >= Txt_Start_Line + disp_height - Least_V_Distance && Txt_Start_Line + disp_height < line_no){
        Txt_Start_Line = Line_Pos - (disp_height - Least_V_Distance) + 1;
    }else if(Line_Pos <= Txt_Start_Line + Least_V_Distance && Txt_Start_Line > 0){
        if(Line_Pos < Least_V_Distance){
            Txt_Start_Line = 0;
        }else{
            Txt_Start_Line = Line_Pos - Least_V_Distance;
        }
    }

    Cursor_Y = Txt_Disp_Start_Y + (Line_Pos - Txt_Start_Line);
}

void update_text_display(){
    if(stdscr == NULL) return ;

    if(Current_File == NULL) return ;

    size_t line_no = simple_file_get_line_no(Current_File);
    int log_of_line_no = (int)log10(line_no);
    Indent = log_of_line_no + 1;

    if(Text_Start_X != Txt_Disp_Start_X + Indent) Text_Start_X = Txt_Disp_Start_X + Indent + 1;
    update_values();

    unsigned int row_no = Txt_Disp_End_Y - Txt_Disp_Start_Y;
    unsigned int max_len = Txt_Disp_End_X - Text_Start_X;
    
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


    attron(COLOR_PAIR(TEXT));

    for(unsigned int i = 0 ; i < row_no ; i++){
        if(Txt_Start_Line + i == Line_Pos){
            attroff(COLOR_PAIR(TEXT));

            attron(COLOR_PAIR(LINE_HIGHLIGHT_BACKGROUND));

            mvhline(Txt_Disp_Start_Y + i , Text_Start_X , 0 , Txt_Disp_End_X - Text_Start_X + 1);
            
            attroff(COLOR_PAIR(LINE_HIGHLIGHT_BACKGROUND));

            attron(COLOR_PAIR(LINE_HIGHLIGHT));

            mvprintw(Txt_Disp_Start_Y + i , Text_Start_X , "%s" , rows[i]);
            
            attroff(COLOR_PAIR(LINE_HIGHLIGHT));
            attron(COLOR_PAIR(TEXT));
        }else{
            attroff(COLOR_PAIR(TEXT));
            attron(COLOR_PAIR(BACKGROUND));

            mvhline(Txt_Disp_Start_Y + i , 0 , 0 , Screen_Width);

            attroff(COLOR_PAIR(BACKGROUND));
            attron(COLOR_PAIR(TEXT));

            mvprintw(Txt_Disp_Start_Y + i , Text_Start_X , "%s" , rows[i]);
        }
    }

    attron(COLOR_PAIR(TEXT));

    attron(COLOR_PAIR(SIDE_STRIPS));

    size_t number_len = log_of_line_no + 2;
    char empty_number[number_len];
    char number[number_len];
    memset(empty_number , ' ' , number_len);
    memset(number , ' ' , number_len);
    empty_number[number_len - 1] = '\000';
    number[number_len - 1] = '\000';

    for(unsigned int i = 0 ; i < row_no ; i++){
        if(Txt_Start_Line + i >= line_no){
            mvprintw(Txt_Disp_Start_Y + i , Txt_Disp_Start_X , "%s" , empty_number);
        }else{
            itoa(Txt_Start_Line + i + 1 , number + (log_of_line_no - (int)log10(Txt_Start_Line + i + 1)) , 10);
            mvprintw(Txt_Disp_Start_Y + i , Txt_Disp_Start_X , "%s" , number);
        }
    }

    mvvline(Txt_Disp_Start_Y , Txt_Disp_Start_X + log_of_line_no + 1 , 0 , Screen_Height - Txt_Disp_Start_Y);
    attroff(COLOR_PAIR(SIDE_STRIPS));

    if(Cursor_X == -1 && Cursor_Y == -1){
        Cursor_X = Text_Start_X;
        Cursor_Y = Txt_Disp_Start_Y;
    }

    move(Cursor_Y , Cursor_X);
}