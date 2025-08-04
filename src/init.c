#include "../headers/simple_globals.h"
#include "../headers/curses_header.h"
#include "../headers/display.h"
#include "../headers/init.h"

void init_display_attrs(){
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
}

void init_values(){
    Tab_Size = DEFAULT_TAB_SIZE;

    Screen_Width = getmaxx(stdscr);
    Screen_Height = getmaxy(stdscr);

    Text_Color = DEFAULT_TEXT_COLOR;
    Title_Color = DEFAULT_TITLE_COLOR;
    Outline_Color = DEFAULT_OUTLINE_COLOR;
    Background_Color = DEFAULT_BACKGROUND_COLOR;
    Line_Highlight_Color = DEFAULT_LINE_HIGHLIGHT_COLOR;
	Title_Background_Color = DEFAULT_TITLE_BACKGROUNG_COLOR;
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

    init_color_pairs();
}

void start_display(){
    render_background(stdscr , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);
    refresh();

    Text_Window = newwin(Screen_Height - 1 , Screen_Width - 2 , 0 , 1);
    render_background(Text_Window , 0 , 0 , Screen_Width - 1 , Screen_Height - 1 , BACKGROUND);
    wattrset(Text_Window , COLOR_PAIR(TEXT_BACKGROUND));
    nodelay(Text_Window , true);
    keypad(Text_Window , true);
    wnoutrefresh(Text_Window);

    Lower_Strip_Window = newwin(1 , Screen_Width , Screen_Height - 1 , 0);
    wattrset(Lower_Strip_Window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));
    nodelay(Lower_Strip_Window , true);
    keypad(Lower_Strip_Window , true);
    wnoutrefresh(Lower_Strip_Window);

    Msg_Window = newwin(3 , Screen_Width , Screen_Height - 4 , 0);
    wattrset(Msg_Window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));
    nodelay(Msg_Window , true);
    keypad(Msg_Window , true);

    Std_Panel = new_panel(stdscr);
    Msg_Panel = new_panel(Msg_Window);
    Lower_Strip_Panel = new_panel(Lower_Strip_Window);
    Text_Panel = new_panel(Text_Window);

    hide_panel(Msg_Panel);
}
