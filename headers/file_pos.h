#ifndef FILE_POS
#define FILE_POS

void move_ncols_right(size_t cols);
void move_ncols_left(size_t cols);
void move_nlines_down(size_t lines);
void move_nlines_up(size_t lines);
void move_to_pos(size_t line_index , size_t col_index);

#endif