#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/display.h"

void move_ncols_right(size_t cols){
    if(cols == 0) return ;
    if(Col_Pos + cols > simple_file_get_line_len(Current_File , Line_Pos)){
        if(cols > 1) return ;
    }

    if(cols == 1 && Col_Pos == simple_file_get_line_len(Current_File , Line_Pos)){
        if(Line_Pos == simple_file_get_line_no(Current_File)) return ;

        Line_Pos++;
        Col_Pos = 0;
    }else{
        Col_Pos += cols;
    }

}

void move_ncols_left(size_t cols){
    if(cols == 0) return ;
    if(cols > Col_Pos){
        if(cols > 1) return ; 
    }

    if(cols == 1 && Col_Pos == 0){
        if(Line_Pos == 0) return ;

        Line_Pos --;
        Col_Pos = simple_file_get_line_len(Current_File , Line_Pos) + 1;
    }

    Col_Pos -= cols;
}

void move_nlines_down(size_t lines){
    if(lines == 0) return ;
    if(Line_Pos + lines >= simple_file_get_line_no(Current_File)) return ;

    Line_Pos += lines;
}

void move_nlines_up(size_t lines){
    if(lines == 0) return ;
    if(Line_Pos == 0) return ;

    Line_Pos -= lines;
}

void move_to_pos(size_t line_index , size_t col_index){
    if(line_index == Line_Pos && col_index == Col_Pos) return ;

    if(line_index >= simple_file_get_line_no(Current_File)) return ;
    if(col_index > simple_file_get_line_len(Current_File , line_index)) return ;

    if(line_index != Line_Pos){
        if(line_index > Line_Pos){
            move_nlines_down(line_index - Line_Pos);
        }else{
            move_nlines_up(Line_Pos - line_index);
        }
    }

    if(col_index != Col_Pos){
        if(col_index > Col_Pos){
            move_ncols_right(col_index - Col_Pos);
        }else{
            move_ncols_left(Col_Pos - col_index);
        }
    }
}