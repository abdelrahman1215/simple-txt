#include "../../headers/parse_commands.h"
#include "../../headers/simple_globals.h"
#include "../../headers/get_str.h"

#include <pdcurses.h>

void command_mode(){
    if(Lower_Strip_Window == NULL) return;

    unsigned int win_width = getmaxx(Lower_Strip_Window);
    //unsigned int win_height = getmaxy(Lower_Strip_Window);

    wattron(Lower_Strip_Window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    mvwhline(Lower_Strip_Window , 0 , 0 , 0 , Screen_Width);

    wattroff(Lower_Strip_Window , COLOR_PAIR(LOWER_STRIP_BACKGROUND));

    wattron(Lower_Strip_Window , COLOR_PAIR(LOWER_STRIP_TEXT));

    mvwprintw(Lower_Strip_Window , 0 , 0 , ":");
    
    wattroff(Lower_Strip_Window , COLOR_PAIR(LOWER_STRIP_TEXT));

    char *command = get_str(Lower_Strip_Window , NULL , LOWER_STRIP_BACKGROUND , COMMAND , 1 , win_width - 1 , 0 , 1);
    if(command == NULL) return ;

    parse_command(command);
}