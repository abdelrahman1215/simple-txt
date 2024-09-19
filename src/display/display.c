#include "display_txt.c"

#include <string.h>

void render_background(unsigned int start_x , unsigned int start_y , unsigned int end_x , unsigned int end_y , int color_pair){
    if(stdscr == NULL) return ;
    attron(COLOR_PAIR(color_pair));

    for(unsigned int i = 0 ; i < (end_y - start_y) ; i++){
        mvhline(start_y + i , start_x , 0 , end_x - start_x);
    }

    attroff(COLOR_PAIR(color_pair));
}

void init_color_pairs(){
    if(stdscr == NULL) return ;

    init_pair(TEXT , Text_Color , Background_Color);
    init_pair(TITLE , Title_Color , Background_Color);
    init_pair(BACKGROUND , Background_Color , Background_Color);
    init_pair(LINE_HIGHLIGHT , Text_Color , Line_Highlight_Color);
    init_pair(SIDE_STRIPS , Outline_Color , Outline_Background_Color);
    init_pair(LOWER_STRIP_TEXT , Lower_Strip_Text_Color , Lower_Strip_Background_Color);
    init_pair(SIDE_STRIP_HIGHLIGHT , Outline_Highlight_Color , Outline_Background_Color);
    init_pair(LOWER_STRIP_BACKGROUND , Lower_Strip_Background_Color , Lower_Strip_Background_Color);
}

void update_lower_strip(){
    if(stdscr == NULL) return ;

    attron(COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    mvhline(Screen_Height - 1 , 0 , 0 , Screen_Width);

    attroff(COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    attron(A_UNDERLINE);
    attron(COLOR_PAIR(LOWER_STRIP_TEXT));

    //mvvline(Screen_Height - 1 , 0 , 0 , 1);
    switch(Current_Mode){
        case Insert_Mode :
            mvprintw(Screen_Height - 1 , 1 , "Insert mode");
            break;

        case Normal_Mode :
            mvprintw(Screen_Height - 1 , 1 , "Normal mode");
            break;
    }
    attroff(A_UNDERLINE);

    size_t line_pos = simple_file_get_curr_line(Current_File) , col_pos = simple_file_get_curr_column(Current_File);
    int log_of_line_pos = (int)log10(line_pos + 1) , log_of_col_pos = (int)log10(col_pos + 1);

    if(log_of_col_pos + log_of_line_pos + 24/*the extra 12 are for the mode*/ < Screen_Width){
        mvprintw(Screen_Height - 1 , Screen_Width - (log_of_col_pos + log_of_line_pos + 12) , "Ln %i , Col %i" , line_pos + 1 , col_pos + 1);
    }

    attroff(COLOR_PAIR(LOWER_STRIP_TEXT));
}