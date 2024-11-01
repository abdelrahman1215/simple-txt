#include "../headers/simple_globals.h"
#include "../headers/curses_header.h"
#include "../headers/init_display.h"
#include "../headers/display.h"

void init_display(){
    if(stdscr != NULL) return ;

    initscr();
    keypad(stdscr , true);
    nodelay(stdscr , true);
    notimeout(stdscr , true);
    start_color();
    noecho();
    cbreak();
    curs_set(0);
    refresh();

    Tab_Size = DEFAULT_TAB_SIZE;

    Screen_Width = getmaxx(stdscr);
    Screen_Height = getmaxy(stdscr);

    Text_Color = DEFAULT_TEXT_COLOR;
    Title_Color = DEFAULT_TITLE_COLOR;
    Outline_Color = DEFAULT_OUTLINE_COLOR;
    Background_Color = DEFAULT_BACKGROUND_COLOR;
    Line_Highlight_Color = DEFAULT_LINE_HIGHLIGHT_COLOR;
    Outline_Highlight_Color = DEFAULT_OUTLINE_HIGHLIGHT_COLOR;

    Mode_Text_Color = DEFAULT_MODE_TEXT_COLOR;
    Mode_Background_Color = DEFAULT_MODE_BACKGROUND_COLOR;
    Command_Text_Color = DEFAULT_COMMAND_TEXT_COLOR;
    Lower_Strip_Text_Color = DEFAULT_LOWER_STRIP_TEXT_COLOR;
    Lower_Strip_Background_Color = DEFAULT_LOWER_STRIP_BACKGROUND_COLOR;

    Message_Text_Color = DEFAULT_MESSAGE_TEXT_COLOR;
    Message_Background_Color = DEFAULT_MESSAGE_BACKGROUND_COLOR;


    Txt_Disp_Start_X = 0;
    Txt_Disp_Start_Y = 0;
    Txt_Disp_End_X = Screen_Width - 1;
    Txt_Disp_End_Y = Screen_Height - 1;
    Least_H_Distance = 5;
    Least_V_Distance = 3;

    Relative_Line_Number = false;

    Hist_Size = DEFAULT_HIST_SIZE;

    init_color_pairs();
}