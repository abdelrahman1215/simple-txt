#include "../headers/parse_term_args.h"
#include "../headers/simple_globals.h"
#include "../headers/curses_header.h"
#include "../headers/init_display.h"
#include "../headers/msg_fmt.h"
#include "../headers/display.h"
#include "../headers/input.h"

#include <stdlib.h>

//reminder : when using ctrl - v results may vary , in windows terminal
//it captures it then sends the copied text to the input stream
//while in other terminals you will get a ^V

int main(int argc , char **argv){
    parse_term_args(argc , argv);
    display_messages();

    if(Current_File == NULL){
        if(stdscr != NULL){
            endwin();
        }

        return 0;
    }

    Current_Mode = Normal_Mode;
    Quit = false;

    if(stdscr == NULL){
        init_display();
    }

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

    PANEL *Top_Panel;
    while(!Quit){
        display_messages();

        update_lower_strip(Lower_Strip_Window);
        render_background(stdscr , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);
        update_text_display(Current_File , save_text_info , Text_Window , BACKGROUND , TEXT , LINE_HIGHLIGHT , SIDE_STRIPS , SIDE_STRIP_HIGHLIGHT , Least_V_Distance , Least_H_Distance , true , false , true , true , true , 0 , 0 , 0 , 0);

        update_panels();
        doupdate();

        Top_Panel = panel_below(NULL);
        Current_Win = panel_window(Top_Panel);
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