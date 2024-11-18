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

    Hist_Size = DEFAULT_HIST_SIZE;

    text_display_info *save_text_info = new_text_disp_info();

    PANEL *Top_Panel;
    while(!Quit){
        update_lower_strip(Lower_Strip_Window);
        render_background(stdscr , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);
        update_text_display(Current_File , save_text_info , Text_Window , BACKGROUND , TEXT , LINE_HIGHLIGHT , SIDE_STRIPS , SIDE_STRIP_HIGHLIGHT , Least_V_Distance , Least_H_Distance , true , Relative_Line_Number , true , true , true , 0 , 0 , 0 , 0);
        display_messages();

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