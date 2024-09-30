#include "../headers/parse_term_args.h"
#include "../headers/simple_globals.h"
#include "../headers/msg_fmt.h"
#include "../headers/display.h"
#include "../headers/input.h"

#include <pdcurses.h>
#include <stdlib.h>

//reminder : when using ctrl - v results may vary , in windows terminal
//it captures it then sends the copied text to the input stream
//while in other terminals you will get a ^V

int main(int argc , char **argv){
    parse_term_args(argc , argv);
    display_messages();
    
    Current_Mode = Normal_Mode;
    Quit = false;

    initscr();
    keypad(stdscr , true);
    nodelay(stdscr , true);
    notimeout(stdscr , true);
    start_color();
    noecho();
    cbreak();
    curs_set(0);
    refresh();

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

    init_color_pairs();
    render_background(stdscr , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);
    refresh();

    Text_Window = newwin(Screen_Height - 1 , Screen_Width - 2 , 0 , 1);
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
    text_display_info *save_text_info = new_text_disp_info();

    PANEL *top_panel;
    WINDOW *Current_Win;
    while(!Quit){
        display_messages();

        update_lower_strip(Lower_Strip_Window);
        render_background(stdscr , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);
        update_text_display(Current_File , save_text_info , Text_Window , BACKGROUND , TEXT , LINE_HIGHLIGHT , SIDE_STRIPS , SIDE_STRIP_HIGHLIGHT , Least_V_Distance , Least_H_Distance , true , true , true , false , true , 0 , 0 , 0 , 0);

        update_panels();
        doupdate();

        top_panel = panel_below(NULL);
        Current_Win = panel_window(top_panel);
        handle_input(Current_Win);
    }

    free(save_text_info);

    del_panel(Std_Panel);
    del_panel(Msg_Panel);
    del_panel(Text_Panel);
    del_panel(Lower_Strip_Panel);

    delwin(Text_Window);
    delwin(Msg_Window);
    delwin(Lower_Strip_Window);
    endwin();

    destroy_simple_file(Current_File);
}