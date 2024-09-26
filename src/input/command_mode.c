#include "../../headers/parse_commands.h"
#include "../../headers/simple_globals.h"
#include "../../headers/get_str.h"

#include <pdcurses.h>
#include <stdlib.h>
#include <string.h>

autocomp_info autocomplete(char *input , size_t pos){
    autocomp_info ret;

    for(ret.replace_end = pos ; input[ret.replace_end + 1] != '\000' ; ret.replace_end ++){
        if(input[ret.replace_end + 1] == ' ') break;
    }

    for(ret.replace_start = pos ; ret.replace_start > 0 ; ret.replace_start --){
        if(input[ret.replace_start - 1] == ' ') break;
    }

    if(ret.replace_start == ret.replace_end) return (autocomp_info){.replace_start = 0 , .replace_end = 0 , .replacement = 0};

    {
        size_t tok_len = ret.replace_end - ret.replace_start;
        char tmp[tok_len + 1];
        memset(tmp , 0 , tok_len + 1);
        strncpy(tmp , input + ret.replace_start , tok_len);

        ret.replacement = get_nearest_command(tmp);
        if(ret.replacement == NULL) return (autocomp_info){.replace_start = 0 , .replace_end = 0 , .replacement = NULL};
        if(strlen(ret.replacement) == tok_len) return (autocomp_info){.replace_start = 0 , .replace_end = 0 , .replacement = NULL};
    }

    return ret;
}

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

    char *command = get_str(Lower_Strip_Window , autocomplete , LOWER_STRIP_BACKGROUND , COMMAND , 1 , win_width - 1 , 0 , 1);
    if(command == NULL) return ;

    parse_command(command);
}