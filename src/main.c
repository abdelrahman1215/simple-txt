#include "../headers/parse_term_args.h"
#include "../headers/simple_globals.h"
#include "../headers/display.h"
#include "../headers/input.h"

#include <pdcurses.h>
#include <stdlib.h>

//reminder : when using ctrl - v results may vary , in windows terminal
//it captures it then sends the copied text to the input stream
//while in other terminals you will get a ^V

int main(int argc , char **argv){
    parse_term_args(argc , argv);
    Current_Mode = Normal_Mode;
    Quit = false;

    initscr();
    keypad(stdscr , true);
    nodelay(stdscr , true);
    notimeout(stdscr , true);
    start_color();
    noecho();
    refresh();


    Screen_Width = getmaxx(stdscr);
    Screen_Height = getmaxy(stdscr);


    Text_Color = DEFAULT_TEXT_COLOR;
    Title_Color = DEFAULT_TITLE_COLOR;
    Outline_Color = DEFAULT_OUTLINE_COLOR;
    Mode_Text_Color = DEFAULT_MODE_TEXT_COLOR;
    Background_Color = DEFAULT_BACKGROUND_COLOR;
    Command_Text_Color = DEFAULT_COMMAND_TEXT_COLOR;
    Line_Highlight_Color = DEFAULT_LINE_HIGHLIGHT_COLOR;
    Mode_Background_Color = DEFAULT_MODE_BACKGROUND_COLOR;
    Lower_Strip_Text_Color = DEFAULT_LOWER_STRIP_TEXT_COLOR;
    Outline_Highlight_Color = DEFAULT_OUTLINE_HIGHLIGHT_COLOR;
    Lower_Strip_Background_Color = DEFAULT_LOWER_STRIP_BACKGROUND_COLOR;


    Txt_Disp_Start_X = 0;
    Txt_Disp_Start_Y = 0;
    Txt_Disp_End_X = Screen_Width - 1;
    Txt_Disp_End_Y = Screen_Height - 1;
    Least_H_Distance = Least_V_Distance = 2;


    init_color_pairs();
    render_background(stdscr , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);
    refresh();

    Text_Window = newwin(Screen_Height - 1 , Screen_Width - 2 , 0 , 1);
    notimeout(Text_Window , true);
    nodelay(Text_Window , true);
    keypad(Text_Window , true);
    wrefresh(Text_Window);

    Lower_Strip_Window = newwin(1 , Screen_Width , Screen_Height - 1 , 0);
    nodelay(Lower_Strip_Window , true);
    wrefresh(Lower_Strip_Window);
    refresh();


    update_lower_strip(Lower_Strip_Window);
    wrefresh(Lower_Strip_Window);

    text_display_info *save_text_info = new_text_disp_info();
    render_background(Text_Window , 0 , 0 , getmaxx(Text_Window) , Screen_Height - 1 , BACKGROUND);
    update_text_display(Current_File , save_text_info , Text_Window , BACKGROUND , TEXT , LINE_HIGHLIGHT , SIDE_STRIPS , SIDE_STRIP_HIGHLIGHT, Least_V_Distance , Least_H_Distance , true , true , true , false , 0 , 0 , 0 , 0);
    wrefresh(Text_Window);


    while(!Quit){
        handle_input(Text_Window);

        update_lower_strip(Lower_Strip_Window);
        wnoutrefresh(Lower_Strip_Window);

        update_text_display(Current_File , save_text_info , Text_Window , BACKGROUND , TEXT , LINE_HIGHLIGHT , SIDE_STRIPS , SIDE_STRIP_HIGHLIGHT , Least_V_Distance , Least_H_Distance , true , true , true , false , 0 , 0 , 0 , 0);
        wnoutrefresh(Text_Window);

        doupdate();
    }

    free(save_text_info);
    delwin(Lower_Strip_Window);
    delwin(Text_Window);
    endwin();

    destroy_simple_file(Current_File);
}