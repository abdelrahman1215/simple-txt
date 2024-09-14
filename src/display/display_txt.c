#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/display.h"

#include <pdcurses.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct text_display_info{
    simple_file *file;
    size_t line_pos , col_pos;
    size_t start_line , start_col;

    unsigned int cursor_x , cursor_y;
    unsigned int disp_start_x , disp_start_y;
    unsigned int disp_end_x , disp_end_y;
    unsigned int txt_start_x , txt_start_y;
    unsigned int indent;

    bool display_line_no , display_file_name;
}text_display_info;

void update_values(text_display_info *info_ptr){
    info_ptr -> line_pos = simple_file_get_curr_line(info_ptr -> file) , info_ptr -> col_pos = simple_file_get_curr_column(info_ptr -> file);
    size_t line_len = simple_file_get_line_len(info_ptr -> file , info_ptr -> line_pos);
    size_t line_no = simple_file_get_line_no(info_ptr -> file);

    if(info_ptr -> display_line_no){
        int log_of_line_no = (int)log10(line_no);
        info_ptr -> indent = log_of_line_no + 1;
    }else{
        info_ptr -> indent = 0;
    }

    info_ptr -> txt_start_x = info_ptr -> disp_start_x + info_ptr -> indent + (info_ptr -> display_file_name == true);
    info_ptr -> txt_start_y = info_ptr -> disp_start_y + info_ptr -> display_file_name;

    unsigned int disp_width = info_ptr -> disp_end_x - info_ptr -> txt_start_x;


    if(info_ptr -> col_pos >= info_ptr -> start_col + disp_width - Least_H_Distance){
        if(info_ptr -> start_col + disp_width >= line_len){
            info_ptr -> start_col = line_len - disp_width;
        }else{
            info_ptr -> start_col = info_ptr -> col_pos - (disp_width - Least_H_Distance);
        }
    }else if(info_ptr -> col_pos <= info_ptr -> start_col + Least_H_Distance && info_ptr -> start_col > 0){
        if(info_ptr -> col_pos < Least_H_Distance){
            info_ptr -> start_col = 0;
        }else{
            info_ptr -> start_col = info_ptr -> col_pos - Least_H_Distance;
        }
    }

    info_ptr -> cursor_x = (info_ptr -> col_pos - info_ptr -> start_col) + info_ptr -> txt_start_x;

    unsigned int disp_height = info_ptr -> disp_end_y - info_ptr -> txt_start_y;

    if(info_ptr -> line_pos >= info_ptr -> start_line + disp_height - Least_V_Distance){
        if(info_ptr -> start_line + disp_height >= line_no){
            info_ptr -> start_line = line_no - disp_height;
        }else{
            info_ptr -> start_line = info_ptr -> line_pos - (disp_height - Least_V_Distance) + 1;
        }
    }else if(info_ptr -> line_pos <= info_ptr -> start_line + Least_V_Distance && info_ptr -> start_line > 0){
        if(info_ptr -> line_pos < Least_V_Distance){
            info_ptr -> start_line = 0;
        }else{
            info_ptr -> start_line = info_ptr -> line_pos - Least_V_Distance;
        }
    }

    info_ptr -> cursor_y = info_ptr -> txt_start_y + (info_ptr -> line_pos - info_ptr -> start_line);
}

void disp_file_name(text_display_info *info_ptr){
    if(info_ptr -> file == NULL) return ;

    char *file_path;
    {
        const char *tmp = (char *)simple_file_get_name(info_ptr -> file);
        if(tmp == NULL){
            return;
        }

        size_t path_len = strlen(tmp) + 1;

        file_path = calloc(path_len + 1 , sizeof(char));
        strncpy(file_path , tmp , path_len);
    }

    char *file_name = strtok(file_path , "/\\");
    for(char *tmp = strtok(NULL , "/\\") ; tmp != NULL ; file_name = tmp , tmp = strtok(NULL , "/\\")){}

    size_t name_len = strlen(file_name);
    if(name_len + 6 > (info_ptr -> disp_end_x - info_ptr -> disp_start_x)){
        if((info_ptr -> disp_end_x - info_ptr -> disp_start_x) < 6){
            free(file_path);

            return;
        }
        file_name[(info_ptr -> disp_end_x - info_ptr -> disp_start_x) - 6] = '\000';
    }

    attron(COLOR_PAIR(TITLE));

    curs_set(0);

    mvprintw(info_ptr -> disp_start_y , info_ptr -> disp_start_x + ((info_ptr -> disp_end_x - info_ptr -> disp_start_x)/2) - (strlen(file_name)/2) , "%s" , file_name);
    
    curs_set(1);

    attroff(COLOR_PAIR(TITLE));

    free(file_path);
}

void update_text_display(simple_file *file_ptr , bool display_line_no , bool display_file_name , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned end_y){
    if(stdscr == NULL) return ;
    if(file_ptr == NULL) return ;
    if(start_x >= end_x || start_y >= end_y || (start_y + 1 > end_y && display_file_name == true)) return ;
    if(end_x > Screen_Width || end_y > Screen_Height) return ;

    static simple_file *last_displayed_file = NULL;

    static text_display_info info;
    if(file_ptr != last_displayed_file){
        info.start_col = 0;
        info.start_line = 0;
        info.file = file_ptr;

        last_displayed_file = file_ptr;
    }

    info.display_line_no = display_line_no;
    info.display_file_name = display_file_name;

    info.disp_start_x = start_x;
    info.disp_end_x = end_x;

    info.disp_start_y = start_y;
    info.disp_end_y = end_y;

    size_t line_no = simple_file_get_line_no(file_ptr);
    int log_of_line_no = (int)log10(line_no);

    if(log_of_line_no + 2 >= end_x - start_x) info.display_line_no = false;

    update_values(&info);

    if(info.txt_start_x >= end_x || info.txt_start_y >= end_y) return ;

    unsigned int row_no = info.disp_end_y - info.txt_start_y;
    size_t max_len = info.disp_end_x - info.txt_start_x;
    
    char rows[row_no][max_len + 1];
    for(unsigned int i = 0 ; i < row_no ; i++){
        memset(rows[i] , ' ' , max_len + 1);
        rows[i][max_len] = '\000';
    }

    char *tmp = NULL;
    unsigned int used_rows = 0;
    size_t line_len;
    for(unsigned int i = 0 ; i < row_no ; i++ , used_rows++){
        tmp = simple_file_get_line(file_ptr , i + info.start_line);
        if(tmp == NULL) break;
        line_len = simple_file_get_line_len(file_ptr , i + info.start_line);
        if(info.start_col >= line_len) continue;

        strncpy(rows[i] , tmp + info.start_col , max_len > (line_len - info.start_col) ? (line_len - info.start_col) : max_len);

        free(tmp);
    }

    for(unsigned int i = used_rows ; i < row_no ; i++){
        rows[i][0] = '~';
    }

    attron(COLOR_PAIR(BACKGROUND));

    for(unsigned int i = 0 ; i < (info.disp_end_y - info.disp_start_y) ; i++){
        mvhline(info.disp_start_y + i , info.disp_start_x , 0 , info.disp_end_x - info.disp_start_x);
    }

    attroff(COLOR_PAIR(BACKGROUND));

    attron(COLOR_PAIR(TEXT));
    for(unsigned int i = 0 ; i < row_no ; i++){
        if(info.start_line + i == info.line_pos){
            attroff(COLOR_PAIR(TEXT));

            attron(COLOR_PAIR(LINE_HIGHLIGHT));

            mvprintw(info.txt_start_y + i , info.txt_start_x , "%s" , rows[i]);
            
            attroff(COLOR_PAIR(LINE_HIGHLIGHT));
            
            attron(COLOR_PAIR(TEXT));
        }else{
            mvprintw(info.txt_start_y + i , info.txt_start_x , "%s" , rows[i]);
        }
    }

    attroff(COLOR_PAIR(TEXT));

    if(info.display_line_no == true){
        attron(COLOR_PAIR(SIDE_STRIPS));

        size_t number_len = log_of_line_no + 2;
        char empty_number[number_len];
        char number[number_len];
        
        memset(empty_number , ' ' , number_len);
        memset(number , ' ' , number_len);

        empty_number[number_len - 1] = '\000';
        number[number_len - 1] = '\000';

        for(unsigned int i = 0 ; i < row_no ; i++){
            if(info.start_line + i >= line_no){
                mvprintw(info.txt_start_y + i , info.disp_start_x , "%s" , empty_number);
            }else{
                itoa(info.start_line + i + 1 , number + (log_of_line_no - (int)log10(info.start_line + i + 1)) , 10);
                mvprintw(info.txt_start_y + i , info.disp_start_x , "%s" , number);
            }
        }   

        mvvline(info.txt_start_y , info.disp_start_x + log_of_line_no + 1 , 0 , Screen_Height - info.txt_start_y);

        attroff(COLOR_PAIR(SIDE_STRIPS));
    }

    if(display_file_name == true){
        disp_file_name(&info);
    }

    if(info.cursor_x == -1 && info.cursor_y == -1){
        info.cursor_x = info.txt_start_x;
        info.cursor_y = info.txt_start_y;
    }

    move(info.cursor_y , info.cursor_x);

    last_displayed_file = file_ptr;
}