#include "../../headers/parse_commands.h"
#include "../../headers/simple_globals.h"
#include "../../headers/curses_header.h"
#include "../../headers/get_str.h"

#include <stdlib.h>
#include <string.h>

autocomp_info autocomplete(char *input , size_t pos , bool get_replacement){
    autocomp_info ret = {.opt_no = 0 , .rep_options = NULL};

    for(ret.replace_end = pos ; input[ret.replace_end + 1] != '\000' ; ret.replace_end ++){
        if(input[ret.replace_end + 1] == ' ') break;
    }

    for(ret.replace_start = pos ; ret.replace_start > 0 ; ret.replace_start --){
        if(input[ret.replace_start - 1] == ' ') break;
    }

    if(ret.replace_start == ret.replace_end) return (autocomp_info){.replace_start = 0 , .replace_end = 0 , .rep_options = NULL , .opt_no = 0};

    if(get_replacement){
        size_t tok_len = ret.replace_end - ret.replace_start;
        char tok[tok_len + 1];
        memset(tok , 0 , tok_len + 1);
        strncpy(tok , input + ret.replace_start , tok_len);

        linked_list *list = get_nearest_command_list(tok);
        if(list == NULL) return (autocomp_info){.replace_start = 0 , .replace_end = 0 , .rep_options = NULL , .rep_options = NULL};

        ret.opt_no = linked_list_get_node_no(list);
        if(ret.opt_no == 0){
            destroy_linked_list(list);

            return (autocomp_info){.replace_start = 0 , .replace_end = 0 , .rep_options = NULL , .rep_options = NULL};
        }

        ret.rep_options = calloc(ret.opt_no , sizeof(char *));
        if(ret.rep_options ==NULL){
            destroy_linked_list(list);

            return (autocomp_info){.replace_start = 0 , .replace_end = 0 , .rep_options = NULL , .rep_options = NULL};
        }

        command_info *target_info;
        size_t command_len;
        size_t i = 0;
        for(node *target_node = linked_list_get_first_node(list) ; target_node != NULL ; target_node = linked_list_get_next_node(target_node)){
            target_info = linked_list_get_obj(target_node);

            command_len = strlen(target_info -> command);
            if(command_len == tok_len && i == 0){
                ret.rep_options[ret.opt_no - 1] = calloc(command_len + 1 , sizeof(char));
                strncpy(ret.rep_options[ret.opt_no - 1] , target_info -> command , command_len);   
            }else{
                ret.rep_options[i] = calloc(command_len + 1 , sizeof(char));
                strncpy(ret.rep_options[i] , target_info -> command , command_len);
                i++;
            }

            free(target_info);
        }
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