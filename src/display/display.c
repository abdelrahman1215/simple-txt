#include "display_txt.c"
#include "display_msg.c"

#include <string.h>

void render_background(WINDOW *disp_window , unsigned int start_x , unsigned int start_y , unsigned int end_x , unsigned int end_y , int color_pair){
    if(disp_window == NULL) return ;
    wattron(disp_window , COLOR_PAIR(color_pair));

    for(unsigned int i = 0 ; i < (end_y - start_y) ; i++){
        mvwhline(disp_window , start_y + i , start_x , 0 , end_x - start_x);
    }

    wattroff(disp_window , COLOR_PAIR(color_pair));
}

text_display_info *new_text_disp_info(){
    text_display_info *ret = calloc(1 , sizeof(text_display_info));
    if(ret == NULL) return NULL;

    ret -> file = NULL;
    ret -> col_pos = ret -> line_pos = 0;
    ret -> start_line = ret -> start_col = 0;

    ret -> cursor_x = ret -> cursor_y = 0;
    ret -> disp_start_x = ret -> disp_end_x = ret -> disp_start_y = ret -> disp_end_y = ret -> txt_start_x = ret -> txt_start_y = ret -> indent = 0;
    
    ret -> background_pair = ret -> text_pair = ret -> line_highlight_pair = ret -> side_strip_highlight_pair = ret -> side_strip_pair = 0;
    ret -> display_line_no = ret -> display_file_name = ret -> highlight_current_line = false;

    return ret;
}

void init_color_pairs(){
    if(stdscr == NULL) return ;

    init_pair(TEXT , Text_Color , Background_Color);
    init_pair(TITLE , Title_Color , Background_Color);
    init_pair(SIDE_STRIPS , Outline_Color , Background_Color);
    init_pair(BACKGROUND , Background_Color , Background_Color);
    init_pair(TEXT_BACKGROUND , Background_Color , Background_Color);
    init_pair(SIDE_STRIP_HIGHLIGHT , Outline_Highlight_Color , Line_Highlight_Color);

    init_pair(MODE, Mode_Text_Color, Mode_Background_Color);
    init_pair(LINE_HIGHLIGHT , Text_Color , Line_Highlight_Color);
    init_pair(COMMAND , Command_Text_Color , Lower_Strip_Background_Color);
    init_pair(LOWER_STRIP_TEXT , Lower_Strip_Text_Color , Lower_Strip_Background_Color);
    init_pair(LOWER_STRIP_BACKGROUND , Lower_Strip_Background_Color , Lower_Strip_Background_Color);

    init_pair(MESSAGE_E , DEFAULT_E_COLOR , DEFAULT_MESSAGE_BACKGROUND_COLOR);
    init_pair(MESSAGE_W , DEFAULT_W_COLOR , DEFAULT_MESSAGE_BACKGROUND_COLOR);
    init_pair(MESSAGE_M , DEFAULT_M_COLOR , DEFAULT_MESSAGE_BACKGROUND_COLOR);
    init_pair(ERROR_COLOR , DEFAULT_ERROR_COLOR , DEFAULT_MESSAGE_BACKGROUND_COLOR);
    init_pair(WARNING_COLOR , DEFAULT_WARNING_COLOR , DEFAULT_MESSAGE_BACKGROUND_COLOR);
    init_pair(MESSAGE_COLOR , DEFAULT_MESSAGE_COLOR , DEFAULT_MESSAGE_BACKGROUND_COLOR);
    init_pair(MESSAGE_TEXT , Message_Text_Color , Message_Background_Color);
    init_pair(MESSAGE_BACKGROUND , Message_Background_Color , Message_Background_Color);
}

#define MODE_MAX_LEN 11

void display_recorded_binding(WINDOW *disp_window , char *recorded_binding){
    if(stdscr == NULL || recorded_binding == NULL) return ;
    if(recorded_binding[0] == '\000') return ;
    unsigned int win_width = getmaxx(disp_window) , win_height = getmaxy(disp_window);

    wattron(disp_window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    mvwhline(disp_window , win_height - 1 , 0 , ' ' , Screen_Width);

    wattroff(disp_window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));



    wattron(disp_window , COLOR_PAIR(MODE));

    mvwprintw(disp_window , win_height - 1 , 0 , " Recording ");

    wattroff(disp_window , COLOR_PAIR(MODE));

    loading_err get_err;
    simple_file *binding = load_from_str(recorded_binding , &get_err);
    if(get_err != OK) return ;

    update_text_display(binding , NULL , disp_window , LOWER_STRIP_BACKGROUND , LOWER_STRIP_TEXT , 0 , 0 , 0 , 0 , 0 , false , false , false , false , 12 , win_width , 0 , 0);
}

void update_lower_strip(WINDOW *disp_window){
    if(stdscr == NULL) return ;
    unsigned int win_width = getmaxx(disp_window) , win_height = getmaxy(disp_window);

    wattron(disp_window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    mvwhline(disp_window , win_height - 1 , 0 , ' ' , Screen_Width);

    wattroff(disp_window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));



    wattron(disp_window , COLOR_PAIR(MODE));

    switch(Current_Mode){
        case Insert_Mode :
            mvwprintw(disp_window , win_height - 1 , 0 , " Insert ");
            break;

        case Normal_Mode :
            mvwprintw(disp_window , win_height - 1 , 0 , " Normal ");
            break;
    }
    
    wattroff(disp_window , COLOR_PAIR(MODE));



    wattron(disp_window , COLOR_PAIR(LOWER_STRIP_TEXT));

    size_t line_pos = simple_file_get_curr_line(Current_File) , col_pos = simple_file_get_curr_column(Current_File);
    int log_of_line_pos = (int)log10(line_pos + 1) , log_of_col_pos = (int)log10(col_pos + 1);

    if(log_of_col_pos + log_of_line_pos + 11/*for the written text*/ + MODE_MAX_LEN < win_width){
        mvwprintw(disp_window , win_height - 1 , win_width - (log_of_col_pos + log_of_line_pos + 11) , "Ln %i,Col %i" , line_pos + 1 , col_pos + 1);
    }

    wattroff(disp_window , COLOR_PAIR(LOWER_STRIP_TEXT));
}