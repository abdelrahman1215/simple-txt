#ifndef DISPLAY_H
#define DISPLAY_H

void move_ncols_right(size_t cols);
void move_ncols_left(size_t cols);
void move_nlines_down(size_t lines);
void move_nlines_up(size_t lines);
void move_to_pos(size_t line_index , size_t col_index);

void update_text_display();

void update_background();

#endif