#include "../../headers/simple_globals.h"
#include "../../headers/curses_header.h"
#include "../../headers/display.h"
#include "../../headers/msg_fmt.h"
#include "../../headers/init.h"

#include <stdlib.h>

void display_messages(){
    size_t msg_no = get_msg_no();
    if(msg_no == 0) return ;


    char *messages[msg_no];
    size_t msg_lens[msg_no];
    msg_type msg_types[msg_no];
    for(size_t i = 0 ; i < msg_no ; i++){
        messages[i] = dequeue_msg(msg_types + i);
        msg_lens[i] = strlen(messages[i]);
    }

    bool Exit = false;
    bool use_curses = true;
    for(size_t i = 0 ; i < msg_no ; i++){
        if(msg_types[i] == Error){
            use_curses = false;
            Exit = true;

            break;
        }
    }

    if(stdscr == NULL){
        if(use_curses){
            init_display_attrs();
            init_values();
            start_display();
        }else{
            for(size_t i = 0 ; i < msg_no ; i++){
                printf("%s\n" , messages[i]);
            }

            if(Exit){
                exit(1);
            }
        }

    }

    unsigned int max_win_height = (Screen_Height / 2) - 1;
    unsigned int disp_width = Screen_Width - 2;

    size_t line_no = 0;
    for(size_t i = 0 ; i < msg_no ; i++){
        line_no += ((msg_lens[i] + 2/*bec "E:","W:" or "M:" will be added before every msg*/) / disp_width) + 1;
    }

    char lines[line_no][disp_width + 1];
    for(size_t i = 0 ; i < line_no ; i++){
        memset(lines[i] , ' ' , disp_width);
        lines[i][disp_width] = '\000';
    }

    bool is_msg_start_line[line_no];
    memset(is_msg_start_line , false , line_no * sizeof(bool));
    for(size_t i = 0 , curr_line = 0 ; i < msg_no ; i++){
        for(char *ptr = messages[i] ; ptr < messages[i] + msg_lens[i] ; ptr += disp_width - 2){
            if(ptr == messages[i]){
                is_msg_start_line[curr_line] = true;
            }

            size_t cpy_count = msg_lens[i] < disp_width - 2 ? msg_lens[i] : disp_width - 2;
            strncpy(lines[curr_line] + 2 , ptr , cpy_count);
            curr_line ++;
        }
    }

    unsigned int win_height = line_no + 1;//+1 bec "press esc to exit" wil be printed in the upper line
    if(win_height > max_win_height) win_height = max_win_height;

    show_panel(Msg_Panel);

    if(win_height > getmaxy(Msg_Window)){
        mvwin(Msg_Window , Screen_Height - (win_height) , 0);
        wresize(Msg_Window , win_height , Screen_Width);
    }else{
        wresize(Msg_Window , win_height , Screen_Width);
        mvwin(Msg_Window , Screen_Height - (win_height) , 0);
    }

    render_background(Msg_Window  , 0 , 0 , Screen_Width , win_height , MESSAGE_BACKGROUND);

    wattron(Msg_Window , COLOR_PAIR(MESSAGE_TEXT));

    mvwprintw(Msg_Window , 0 , 1 , "press escape to exit");

    wattroff(Msg_Window , COLOR_PAIR(MESSAGE_TEXT));

    size_t start_line = 0;
    char empty_line[disp_width + 1];
    memset(empty_line , ' ' , disp_width);
    empty_line[disp_width] = '\000';

    for(unsigned int i = 0 , j = 0 ; i + 1 < win_height ; i++){
        if(i + start_line > line_no){
            mvwprintw(Msg_Window , i + 1 , 1 , "%s" , empty_line);
            continue;
        }

        switch(msg_types[j]){
            case Error :
                wattron(Msg_Window , COLOR_PAIR(ERROR_COLOR));
                mvwprintw(Msg_Window , i + 1 , 1 , lines[i + start_line]);
                wattroff(Msg_Window , COLOR_PAIR(ERROR_COLOR));

                break;

            case Warning :
                wattron(Msg_Window , COLOR_PAIR(WARNING_COLOR));
                mvwprintw(Msg_Window , i + 1 , 1 , lines[i + start_line]);
                wattroff(Msg_Window , COLOR_PAIR(WARNING_COLOR));

                break;

            case Message :
                wattron(Msg_Window , COLOR_PAIR(MESSAGE_COLOR));
                mvwprintw(Msg_Window , i + 1 , 1 , lines[i + start_line]);
                wattroff(Msg_Window , COLOR_PAIR(MESSAGE_COLOR));

                break;
        }

        if(is_msg_start_line[i + start_line]){
            if(i > 0) j++;

            switch(msg_types[j]){
                case Error :
                    wattron(Msg_Window , COLOR_PAIR(MESSAGE_E));
                    mvwprintw(Msg_Window , i + 1 , 1 , "E:");
                    wattroff(Msg_Window , COLOR_PAIR(MESSAGE_E));

                    break;

                case Warning :
                    wattron(Msg_Window , COLOR_PAIR(MESSAGE_W));
                    mvwprintw(Msg_Window , i + 1 , 1 , "W:");
                    wattroff(Msg_Window , COLOR_PAIR(MESSAGE_W));

                    break;

                case Message :
                    wattron(Msg_Window , COLOR_PAIR(MESSAGE_M));
                    mvwprintw(Msg_Window , i + 1 , 1 , "M:");
                    wattroff(Msg_Window , COLOR_PAIR(MESSAGE_M));

                    break;
            }
        }
    }

    update_panels();
    doupdate();

    for(int ch = wgetch(Msg_Window) ; ch != '\e' ; ch = wgetch(Msg_Window)){
        if(ch == ERR) continue;

        switch(ch){
            case KEY_UP:
                if(start_line > 0){
                    start_line --;
                }

                break;

            case KEY_DOWN:
                if(start_line + (win_height - 1) < line_no){
                    start_line ++;
                }

                break;
        }

        size_t j = 0;
        for(size_t k = 0 ; k < start_line ; k++){
            if(is_msg_start_line[k] == true){
                j++;
            }
        }

        for(unsigned int i = 0 ; i + 1 < win_height ; i++){
            if(i + start_line > line_no){
                mvwprintw(Msg_Window , i + 1 , 1 , "%s" , empty_line);
                continue;
            }

            switch(msg_types[j]){
                case Error :
                    wattron(Msg_Window , COLOR_PAIR(ERROR_COLOR));
                    mvwprintw(Msg_Window , i + 1 , 1 , lines[i + start_line]);
                    wattroff(Msg_Window , COLOR_PAIR(ERROR_COLOR));

                    break;

                case Warning :
                    wattron(Msg_Window , COLOR_PAIR(WARNING_COLOR));
                    mvwprintw(Msg_Window , i + 1 , 1 , lines[i + start_line]);
                    wattroff(Msg_Window , COLOR_PAIR(WARNING_COLOR));

                    break;

                case Message :
                    wattron(Msg_Window , COLOR_PAIR(MESSAGE_COLOR));
                    mvwprintw(Msg_Window , i + 1 , 1 , lines[i + start_line]);
                    wattroff(Msg_Window , COLOR_PAIR(MESSAGE_COLOR));

                    break;

            }

            if(is_msg_start_line[i + start_line]){
                if(i > 0) j++;

                switch(msg_types[j]){
                    case Error :
                        wattron(Msg_Window , COLOR_PAIR(MESSAGE_E));
                        mvwprintw(Msg_Window , i + 1 , 1 , "E:");
                        wattroff(Msg_Window , COLOR_PAIR(MESSAGE_E));
    
                        break;
    
                    case Warning :
                        wattron(Msg_Window , COLOR_PAIR(MESSAGE_W));
                        mvwprintw(Msg_Window , i + 1 , 1 , "W:");
                        wattroff(Msg_Window , COLOR_PAIR(MESSAGE_W));
    
                        break;
    
                    case Message :
                        wattron(Msg_Window , COLOR_PAIR(MESSAGE_M));
                        mvwprintw(Msg_Window , i + 1 , 1 , "M:");
                        wattroff(Msg_Window , COLOR_PAIR(MESSAGE_M));
    
                        break;
                }
            }
        }

        update_panels();
        doupdate();
    }

    wattroff(Msg_Window , COLOR_PAIR(MESSAGE_TEXT));

    hide_panel(Msg_Panel);
    update_panels();
    doupdate();
}