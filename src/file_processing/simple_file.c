#include "edit_file.c"
#include "load_file.c"

void simple_file_move_ncols_right(simple_file *file_ptr , size_t cols){
    if(file_ptr == NULL) return ;
    if(file_ptr -> column + cols > simple_file_get_line_len(file_ptr , file_ptr -> line)){
        if(cols > 1){
            file_ptr -> column = simple_file_get_line_len(file_ptr , file_ptr -> line);
        }

        if(file_ptr -> max_reached_column < file_ptr -> column) file_ptr -> max_reached_column = file_ptr -> column; 

        return;
    }

    file_ptr -> column += cols;
    if(file_ptr -> max_reached_column < file_ptr -> column) file_ptr -> max_reached_column = file_ptr -> column; 
}

void simple_file_move_ncols_left(simple_file *file_ptr , size_t cols){
    if(file_ptr == NULL) return ;
    if(file_ptr -> column < cols){
        if(cols > 1){
            file_ptr -> column = 0;
            file_ptr -> max_reached_column = file_ptr -> column;
        }


        return;
    }

    file_ptr -> column -= cols;
    file_ptr -> max_reached_column = file_ptr -> column;
}

void simple_file_move_nlines_down(simple_file *file_ptr , size_t lines){
    if(file_ptr == NULL) return ;
    if(file_ptr -> line + lines >= simple_file_get_line_no(file_ptr)){
        if(lines > 1){
            file_ptr -> line = simple_file_get_line_no(file_ptr);
        }

        return;
    }

    file_ptr -> line += lines;

    size_t line_len = simple_file_get_line_len(file_ptr , file_ptr -> line);
    if(file_ptr -> column > line_len){
        file_ptr -> column = line_len;
    }else if(file_ptr -> column < file_ptr -> max_reached_column){
        if(line_len >= file_ptr -> max_reached_column) file_ptr -> column = file_ptr -> max_reached_column;
        else file_ptr -> column = line_len;
    }
}

void simple_file_move_nlines_up(simple_file *file_ptr , size_t lines){
    if(file_ptr == NULL) return ;
    if(file_ptr -> line < lines){
        if(lines > 1){
            file_ptr -> line = 0;
        }

        return;
    }

    file_ptr -> line -= lines;

    size_t line_len = simple_file_get_line_len(file_ptr , file_ptr -> line);
    if(file_ptr -> column > line_len){
        file_ptr -> column = line_len;
    }else if(file_ptr -> column < file_ptr -> max_reached_column){
        if(line_len >= file_ptr -> max_reached_column) file_ptr -> column = file_ptr -> max_reached_column;
        else file_ptr -> column = line_len;
    }
}

void simple_file_move_to_xy(simple_file *file_ptr , size_t line_index , size_t column_index){
    if(file_ptr == NULL) return ;
    if(line_index >= simple_file_get_line_no(file_ptr)) return ;
    if(column_index > simple_file_get_line_len(file_ptr , line_index)) return ;

    file_ptr -> line = line_index;
    file_ptr -> column = column_index;

    if(file_ptr -> max_reached_column < file_ptr -> column) file_ptr -> max_reached_column = file_ptr -> column; 
}

size_t simple_file_get_curr_line(simple_file *file_ptr){
    if(file_ptr == NULL) return 0;

    return file_ptr -> line;
}

size_t simple_file_get_curr_column(simple_file *file_ptr){
    if(file_ptr == NULL) return 0;

    return file_ptr -> column;
}