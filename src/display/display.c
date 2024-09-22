#include "display_txt.c"

#include <string.h>

void render_background(WINDOW *disp_window , unsigned int start_x , unsigned int start_y , unsigned int end_x , unsigned int end_y , int color_pair){
    if(disp_window == NULL) return ;
    wattron(disp_window , COLOR_PAIR(color_pair));

    for(unsigned int i = 0 ; i < (end_y - start_y) ; i++){
        mvwhline(disp_window , start_y + i , start_x , 0 , end_x - start_x);
    }

    wattroff(disp_window , COLOR_PAIR(color_pair));
}

void init_color_pairs(){
    if(stdscr == NULL) return ;

    init_pair(TEXT , Text_Color , Background_Color);
    init_pair(TITLE , Title_Color , Background_Color);
    init_pair(BACKGROUND , Background_Color , Background_Color);
    init_pair(LINE_HIGHLIGHT , Text_Color , Line_Highlight_Color);
    init_pair(SIDE_STRIPS , Outline_Color , Outline_Background_Color);
    init_pair(SIDE_STRIP_HIGHLIGHT , Outline_Highlight_Color , Line_Highlight_Color);
    init_pair(LOWER_STRIP_TEXT , Lower_Strip_Text_Color , Lower_Strip_Background_Color);
    init_pair(LOWER_STRIP_BACKGROUND , Lower_Strip_Background_Color , Lower_Strip_Background_Color);
}

void update_lower_strip(WINDOW *disp_window){
    if(stdscr == NULL) return ;
    unsigned int win_width = getmaxx(disp_window) ; //win_height = getmaxy(disp_window);

    wattron(disp_window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    mvwhline(disp_window , 0 , 0 , 0 , Screen_Width);

    wattroff(disp_window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    wattron(disp_window , COLOR_PAIR(LOWER_STRIP_TEXT));
    wattron(disp_window , A_UNDERLINE);

    switch(Current_Mode){
        case Insert_Mode :
            mvwprintw(disp_window , 0 , 1 , "Insert");
            break;

        case Normal_Mode :
            mvwprintw(disp_window , 0 , 1 , "Normal");
            break;
    }
    wattroff(disp_window , A_UNDERLINE);

    size_t line_pos = simple_file_get_curr_line(Current_File) , col_pos = simple_file_get_curr_column(Current_File);
    int log_of_line_pos = (int)log10(line_pos + 1) , log_of_col_pos = (int)log10(col_pos + 1);

    if(log_of_col_pos + log_of_line_pos + 17/*the extra 6 are for the mode*/ < win_width){
        mvwprintw(disp_window , 0 , win_width - (log_of_col_pos + log_of_line_pos + 11) , "Ln %i,Col %i" , line_pos + 1 , col_pos + 1);
    }

    wattroff(disp_window , COLOR_PAIR(LOWER_STRIP_TEXT));
}