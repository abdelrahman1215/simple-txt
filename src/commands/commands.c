#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/open_files.h"
#include "../../headers/display.h"
#include "../../headers/msg_fmt.h"
#include "../../headers/manual.h"

#include <stdlib.h>
#include <unistd.h>

//command : "w"
//tokens_no : 0
void write_to_current_file(char **args){
    if(simple_file_changes_saved(Current_File)) return ;

    save_file(Current_File);
    wrote_into_file_msg(simple_file_get_name(Current_File));

    return ;
}

//command : "q"
//tokens_no : 0
void quit(char **args){
    if(!simple_file_changes_saved(Current_File)){
        custom_msg(Error , "save unsaved changes first or use q!");
        return;
    }

    Quit = true;

    return ;
}

//command : "q!"
//tokens_no : 0
void forced_quit(char **args){
    Quit = true;

    return ;
}

//command : "wq"
//tokens_no : 0
void write_and_quit(char **args){
    write_to_current_file(args);
    quit(args);

    return ;
}

//command : "edit"
//tokens_no : 1
void edit_file(char **args){
    if(Current_File != NULL){
        if(!simple_file_changes_saved(Current_File)){
            custom_msg(Error , "save current file first or use edit!");
            return ;
        }
    }

    open_file(args[0]);

    return ;
}
//command : "edit!"
//tokens_no : 1
void forced_edit_file(char **args){
    open_file(args[0]);

    return ;
}

//command : "relative"
//tokens : 0
void relative(char **args){
    Relative_Line_Number = !Relative_Line_Number;
}
//command : "ln"
//tokens : 1
void move_to_line(char **args){
    char *str_line_no = args[0];
    size_t line_no = atoi(str_line_no);
    if(line_no == 0 || line_no > simple_file_get_line_no(Current_File)){
        command_msg(Message , "ln" , args[0] , "Invalid line number");

        return;
    }

    size_t curr_line = simple_file_get_curr_line(Current_File);
    if(curr_line > (line_no - 1)) simple_file_move_nlines_up(Current_File , curr_line - (line_no - 1));
    else simple_file_move_nlines_down(Current_File , (line_no - 1) - curr_line);
}

//command : "col"
//tokens : 1
void move_to_column(char **args){
    char *str_col_no = args[0];
    size_t col_no = atoi(str_col_no);
    size_t curr_line = simple_file_get_curr_line(Current_File);
    if(col_no == 0 || col_no > simple_file_get_line_len(Current_File , curr_line) + 1){
        command_msg(Message , "col" , args[0] , "Invalid column number");

        return ;
    }

    simple_file_move_to_xy(Current_File , curr_line , col_no - 1);
}

//command : "man"
//tokens : 0
void manual(char **args){
    char *manual = get_manual();

    unsigned int disp_width = Screen_Width - 2;

    loading_err error;
    simple_file *display_file = load_from_str("" , &error);

    size_t line_no = 0;
    char buffer[disp_width + 1];
    buffer[disp_width] = '\000';
    for(size_t i = 0 , copy_start = 0 , copy_end = 0 , copy_len = 0 ;; i++){
        if(manual[i] == '\000' || manual[i] == '\n' || i - copy_end/*last copy end*/ >= disp_width){
            copy_start = copy_end + (copy_end > 0);
            copy_end = i;

            copy_len = copy_end - copy_start;
            strncpy(buffer , manual + copy_start , copy_len);

            buffer[copy_len] = '\000';

            size_t line_index = simple_file_get_curr_line(display_file);
            size_t line_len = simple_file_get_line_len(display_file , line_index);
            simple_file_add(display_file , line_index , line_len , buffer);
            if(manual[i] != '\000'){
                line_index = simple_file_get_curr_line(display_file);
                line_len = simple_file_get_line_len(display_file , line_index);
                simple_file_add(display_file , line_index , line_len , "\n");
            }
        }

        if(manual[i] == '\000') break;
    }

    simple_file_move_to_xy(display_file , 0 , 0);

    WINDOW *display_win = stdscr;

    render_background(display_win , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);
    
    wattron(display_win , COLOR_PAIR(SIDE_STRIPS));
    
    mvwprintw(display_win , 0 , 1 , "press esc to exit");

    wattroff(display_win , COLOR_PAIR(SIDE_STRIPS));


    text_display_info *info = new_text_disp_info();
    flushinp();

    size_t curr_line;
    size_t file_line_no = simple_file_get_line_no(display_file);
    for(int ch = getch() ; ch != '\e' ; ch = getch()){
        update_text_display(display_file , info , display_win , BACKGROUND , TEXT , 0 , 0 , 0 , 0 , 0 , false , false , false , false , false , 1 , Screen_Width - 1 , 1 , Screen_Height);
        refresh();

        curr_line = simple_file_get_curr_line(display_file);
        switch(ch){
            case KEY_UP:
                simple_file_move_nlines_up(display_file , 1);

                break;

            case KEY_DOWN:
                if(curr_line + Screen_Height - 1 < file_line_no){
                    simple_file_move_nlines_down(display_file , 1);
                }

                break;
        }
    }
}

//command : "pwd"
//tokens : 0
void pwd(char **args){
    char cwd[261] = {'\000'};
    getcwd(cwd , 261);

    command_msg(Message , "cwd" , cwd , NULL);
}

//command : "cd"
//tokens : 1
void change_directory(char **args){
    char *target_path = args[0];
    if(args[0][0] == '.' && (args[0][1] == '\000' || args[0][1] == '\\' || args[0][1] == '/')) return ;

    int err = chdir(target_path);
    
    if(err != 0) command_msg(Error , "cd" , "invalid path" , NULL);
    else pwd(NULL);
}