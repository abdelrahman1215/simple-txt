#include "../../headers/parse_commands.h"
#include "../../headers/simple_globals.h"
#include "../../headers/get_str.h"

#include <pdcurses.h>

void command_mode(){
    if(Lower_Strip_Window == NULL) return;

    unsigned int win_width = getmaxx(Lower_Strip_Window);
    //unsigned int win_height = getmaxy(Lower_Strip_Window);

    mvwprintw(Lower_Strip_Window , 0 , 0 , ":");
    char *command = get_str(Lower_Strip_Window , Lower_Strip_Background_Color , false , 1 , win_width - 1 , 0 , 1);
    if(command == NULL) return ;

    parse_command(command);
}