#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/msg_fmt.h"
#include "../../headers/open_files.h"

#include <stdlib.h>

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
        custom_msg(Error , "ln : Invalid line number");

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
        custom_msg(Error , "col : Invalid col number");

        return ;
    }

    simple_file_move_to_xy(Current_File , curr_line , col_no - 1);
}