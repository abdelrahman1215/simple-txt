#include "../../headers/simple_globals.h"
#include "../../headers/curses_header.h"
#include "../../headers/simple_file.h"
#include "../../headers/display.h"

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct text_display_info{
    simple_file *file;
    size_t line_pos , col_pos;
    size_t start_line , start_col;

    unsigned int cursor_x , cursor_y;
    unsigned int disp_start_x , disp_start_y;
    unsigned int disp_end_x , disp_end_y;
    unsigned int txt_start_x , txt_start_y;
    unsigned int least_v_dist , least_h_dist;
    unsigned int indent;

    unsigned int background_pair , text_pair , line_highlight_pair , side_strip_pair , side_strip_highlight_pair;
    bool display_line_no , relative_line_no , display_file_name , highlight_current_line , display_cursor;

    WINDOW *window;
};

void update_values(text_display_info *info_ptr){
    static simple_file *last_displayed_file = NULL;

    info_ptr -> line_pos = simple_file_get_curr_line(info_ptr -> file) , info_ptr -> col_pos = simple_file_get_curr_column(info_ptr -> file);
    size_t line_len = simple_file_get_line_len(info_ptr -> file , info_ptr -> line_pos);
    size_t line_no = simple_file_get_line_no(info_ptr -> file);

    if(info_ptr -> display_line_no){
        int log_of_line_no = (int)log10(line_no);
        info_ptr -> indent = log_of_line_no + 2 + info_ptr -> highlight_current_line + 1;
    }else{
        info_ptr -> indent = 0;
    }

    info_ptr -> txt_start_x = info_ptr -> disp_start_x + info_ptr -> indent;
    info_ptr -> txt_start_y = info_ptr -> disp_start_y + info_ptr -> display_file_name;

    unsigned int disp_height = info_ptr -> disp_end_y - info_ptr -> txt_start_y;
    unsigned int disp_width = info_ptr -> disp_end_x - info_ptr -> txt_start_x;

    if(info_ptr -> display_cursor){
        if(info_ptr -> col_pos >= info_ptr -> start_col + disp_width - info_ptr -> least_h_dist){
            info_ptr -> start_col = info_ptr -> col_pos - (disp_width - info_ptr -> least_h_dist);
        }else if(info_ptr -> col_pos <= info_ptr -> start_col + info_ptr -> least_h_dist && info_ptr -> start_col > 0){
            if(info_ptr -> col_pos < info_ptr -> least_h_dist){
                info_ptr -> start_col = 0;
            }else{
                info_ptr -> start_col = info_ptr -> col_pos - info_ptr -> least_h_dist;
            }
        }

        info_ptr -> cursor_x = (info_ptr -> col_pos - info_ptr -> start_col) + info_ptr -> txt_start_x;
    }else{
        info_ptr -> start_col = info_ptr -> col_pos;
        if((line_len - info_ptr -> col_pos) + info_ptr -> least_h_dist < disp_width && line_len > disp_width){
            info_ptr -> start_col = line_len - (disp_width) + info_ptr -> least_h_dist;
        }
    }


    if(info_ptr -> highlight_current_line || info_ptr -> display_cursor){
        if(info_ptr -> line_pos >= info_ptr -> start_line + disp_height - info_ptr -> least_v_dist){
            if(info_ptr -> start_line + disp_height + 1 >= line_no && line_no >= disp_height){
                info_ptr -> start_line = line_no - disp_height;
            }else if(line_no < disp_height){
                info_ptr -> start_line = 0;
            }else{
                info_ptr -> start_line = info_ptr -> line_pos - (disp_height - info_ptr -> least_v_dist) + 1;
            }
        }else if(info_ptr -> line_pos <= info_ptr -> start_line + info_ptr -> least_v_dist && info_ptr -> start_line > 0){
            if(info_ptr -> line_pos < info_ptr -> least_v_dist){
                info_ptr -> start_line = 0;
            }else{
                info_ptr -> start_line = info_ptr -> line_pos - info_ptr -> least_v_dist;
            }
        }

        info_ptr -> cursor_y = info_ptr -> txt_start_y + (info_ptr -> line_pos - info_ptr -> start_line);
    }else{
        info_ptr -> start_line = info_ptr -> line_pos;
        if((line_no - info_ptr -> line_pos) < disp_height && line_no > disp_height){
            info_ptr -> start_line = line_no - disp_height;
        }
    }
}

void disp_file_name(text_display_info *info_ptr){
    if(info_ptr -> file == NULL) return ;

    char *file_path;
    {
        const char *tmp = (char *)simple_file_get_name(info_ptr -> file);
        if(tmp == NULL){
            info_ptr -> txt_start_y = info_ptr -> disp_start_y;

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

    wattron(info_ptr -> window , COLOR_PAIR(TITLE));

    mvwprintw(info_ptr -> window , info_ptr -> disp_start_y , info_ptr -> disp_start_x + ((info_ptr -> disp_end_x - info_ptr -> disp_start_x)/2) - (strlen(file_name)/2) , "%s" , file_name);
    
    wattroff(info_ptr -> window , COLOR_PAIR(TITLE));

    free(file_path);
}

void disp_line_no(text_display_info *info_ptr , bool highlight_current_line){
    unsigned int row_no = info_ptr -> disp_end_y - info_ptr -> txt_start_y;
    size_t line_no = simple_file_get_line_no(info_ptr -> file);
    int log_of_line_no = (int)log10(line_no);

    wattron(info_ptr -> window , COLOR_PAIR(info_ptr -> side_strip_pair));

    size_t number_len = log_of_line_no + 1 + highlight_current_line;
    char empty_number[number_len + 1];
    char number[number_len + 1];
        
    memset(empty_number , ' ' , number_len);
    memset(number , ' ' , number_len);

    empty_number[number_len] = '\000';
    number[number_len] = '\000';


    unsigned int x = 0 , y = 0;
    for(unsigned int i = 0 ; i < row_no ; i++){
        x = info_ptr -> disp_start_x + highlight_current_line;
        y = info_ptr -> txt_start_y + i;

        if(info_ptr -> start_line + i == info_ptr -> line_pos && highlight_current_line){
            wattroff(info_ptr -> window , COLOR_PAIR(info_ptr -> side_strip_pair));
            wattron(info_ptr -> window , COLOR_PAIR(info_ptr -> side_strip_highlight_pair));

            x --;
        }

        if(info_ptr -> start_line + i >= line_no){
            mvwprintw(info_ptr -> window , y , x , "%s" , empty_number);
        }else{
            size_t line_number;
            if(info_ptr -> relative_line_no && info_ptr -> start_line + i != info_ptr -> line_pos) line_number = info_ptr -> line_pos > (info_ptr -> start_line + i) ? info_ptr -> line_pos - (info_ptr -> start_line + i) : (info_ptr -> start_line + i) - info_ptr -> line_pos;
            else line_number = info_ptr -> start_line + i + 1;

            sprintf(number + (log_of_line_no - (int)log10(line_number)) + 1 , "%li" , line_number);
            mvwprintw(info_ptr -> window , y , x , "%s" , number);
        }

        if(info_ptr -> start_line + i == info_ptr -> line_pos && highlight_current_line){
            wprintw(info_ptr -> window , "  ");
            wattroff(info_ptr -> window , COLOR_PAIR(info_ptr -> side_strip_highlight_pair));
            wattron(info_ptr -> window , COLOR_PAIR(info_ptr -> side_strip_pair));
        }

        memset(number , ' ' , number_len);
    }   


    wattroff(info_ptr -> window , COLOR_PAIR(info_ptr -> side_strip_pair));
}

void update_text_display(simple_file *file_ptr , text_display_info *save_info , WINDOW *disp_window , unsigned int background_pair , unsigned int text_pair , unsigned int line_highlight_pair , unsigned int side_strip_pair , unsigned int side_strip_highlight_pair , unsigned int least_v_dist , unsigned int least_h_dist , bool display_line_no , bool relative_line_no , bool display_file_name  , bool highlight_current_line , bool disp_cursor , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned end_y){
    if(disp_window == NULL) return ;
    if(file_ptr == NULL) return ;

    bool created_tmp_save_info = false;
    if(save_info == NULL){
        created_tmp_save_info = true;
        save_info = new_text_disp_info();
    }
    
    if(start_x == 0 && end_x == 0){
        end_x = getmaxx(disp_window);
    }

    if(start_y == 0 && end_y == 0){
        end_y = getmaxy(disp_window);
    }

    if((start_x >= end_x && start_x == 0) || (start_y >= end_y && start_y == 0) || (start_y + 1 >= end_y && display_file_name == true)){
        if(created_tmp_save_info) free(save_info);
        
        return ;
    }
    
    if(side_strip_highlight_pair == 0) side_strip_highlight_pair = SIDE_STRIP_HIGHLIGHT;
    if(line_highlight_pair == 0) line_highlight_pair = LINE_HIGHLIGHT;
    if(side_strip_pair == 0) side_strip_pair = SIDE_STRIPS;
    if(background_pair == 0) background_pair = BACKGROUND;
    if(text_pair == 0) text_pair = TEXT;


    unsigned int win_width = getmaxx(disp_window) , win_height = getmaxy(disp_window);
    if(end_x > win_width || end_y > win_height){
        if(created_tmp_save_info) free(save_info);

        return ;
    }

    if(save_info -> file == NULL){
        save_info -> file = file_ptr;
    }else if(save_info -> file != file_ptr){
        save_info -> file = file_ptr;
        save_info -> col_pos = save_info -> line_pos = 0;
        save_info -> start_line = save_info -> start_col = 0;
        save_info -> cursor_x = save_info -> cursor_y = 0;
        save_info -> disp_start_x = save_info -> disp_end_x = save_info -> disp_start_y = save_info -> disp_end_y = save_info -> txt_start_x = save_info -> txt_start_y = save_info -> indent = 0;
        save_info -> background_pair = save_info -> text_pair = save_info -> line_highlight_pair = save_info -> side_strip_highlight_pair = save_info -> side_strip_pair = 0;
        save_info -> display_line_no = save_info -> display_file_name = save_info -> highlight_current_line = false;
    }

    save_info -> file = file_ptr;

    save_info -> display_cursor = disp_cursor;
    save_info -> display_line_no = display_line_no;
    save_info -> relative_line_no = relative_line_no;
    save_info -> display_file_name = display_file_name;
    save_info -> highlight_current_line = highlight_current_line;

    save_info -> disp_start_x = start_x;
    save_info -> disp_end_x = end_x;

    save_info -> disp_start_y = start_y;
    save_info -> disp_end_y = end_y;


    save_info -> least_v_dist = least_v_dist;
    save_info -> least_h_dist = least_h_dist;
    if(save_info -> least_h_dist == 0 && disp_cursor) save_info -> least_h_dist = 1;//for this to work the least_h_dist has to be at least 1

    save_info -> window = disp_window;

    save_info -> side_strip_highlight_pair = side_strip_highlight_pair; 
    save_info -> line_highlight_pair = line_highlight_pair;
    save_info -> side_strip_pair = side_strip_pair;
    save_info -> background_pair = background_pair;
    save_info -> text_pair = text_pair;

    size_t line_no = simple_file_get_line_no(file_ptr);
    int log_of_line_no = (int)log10(line_no);

    if(log_of_line_no + 2 + highlight_current_line >= end_x - start_x) save_info -> display_line_no = false;

    update_values(save_info);

    if(save_info -> txt_start_x >= end_x || save_info -> txt_start_y >= end_y){
        if(created_tmp_save_info) free(save_info);

        return ;
    }

    unsigned int row_no = save_info -> disp_end_y - save_info -> txt_start_y;
    size_t max_len = save_info -> disp_end_x - save_info -> txt_start_x;
    
    char *rows[row_no];
    for(unsigned int i = 0 ; i < row_no ; i++) rows[i] = calloc(max_len + 1 , sizeof(char));

    for(unsigned int i = 0 ; i < row_no ; i++){
        memset(rows[i] , ' ' , max_len);
        //rows[i][max_len] = '\000';
    }

    unsigned int used_rows = 0;
    size_t line_len;
    for(unsigned int i = 0 ; i < row_no && i + save_info -> start_line < line_no ; i++ , used_rows++){
        line_len = simple_file_get_line_len(file_ptr , i + save_info -> start_line);
        if(save_info -> start_col >= line_len) continue;

        simple_file_copy_line(file_ptr , i + save_info -> start_line , save_info -> start_col , rows[i] , max_len + 1);
    }


    render_background(save_info -> window , save_info -> disp_start_x , save_info -> disp_start_y , save_info -> disp_end_x , save_info -> disp_end_y , save_info -> background_pair);

    if(save_info -> display_line_no == true){
        disp_line_no(save_info , highlight_current_line);
    }

    if(display_file_name == true){
        disp_file_name(save_info);
    }

    wattron(save_info -> window , COLOR_PAIR(save_info -> text_pair));
    for(unsigned int i = 0 ; i < used_rows ; i++){
        mvwprintw(save_info -> window , save_info -> txt_start_y + i , save_info -> txt_start_x , "%s" , rows[i]);
    }
    wattroff(save_info -> window , COLOR_PAIR(save_info -> text_pair));

    for(size_t i = 0 ; i < row_no ; i++) free(rows[i]);

    wattron(save_info -> window , COLOR_PAIR(save_info -> side_strip_pair));
    if(used_rows < row_no){
        mvwhline(save_info -> window , save_info -> txt_start_y + used_rows , save_info -> txt_start_x + 1 , 0 , save_info -> disp_end_x - save_info -> txt_start_x - 2);
    }
    wattroff(save_info -> window , COLOR_PAIR(save_info -> side_strip_pair));

    if(highlight_current_line) mvwchgat(save_info -> window , save_info -> cursor_y , save_info -> txt_start_x , (save_info -> disp_end_x - save_info -> txt_start_x) , A_NORMAL , save_info -> line_highlight_pair , NULL);

    if(disp_cursor) mvwchgat(save_info -> window , save_info -> cursor_y , save_info -> cursor_x , 1 , A_REVERSE , save_info -> text_pair , NULL);

    if(save_info -> cursor_x == -1 && save_info -> cursor_y == -1){
        save_info -> cursor_x = save_info -> txt_start_x;
        save_info -> cursor_y = save_info -> txt_start_y;
    }

    wmove(save_info -> window , save_info -> cursor_y , save_info -> cursor_x);


    if(created_tmp_save_info) free(save_info);
}