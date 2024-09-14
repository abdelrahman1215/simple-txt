#ifndef DISPLAY_H
#define DISPLAY_H

#include "simple_file.h"

#include <stdbool.h>

void init_color_pairs();

void move_ncols_right(size_t cols);
void move_ncols_left(size_t cols);
void move_nlines_down(size_t lines);
void move_nlines_up(size_t lines);
void move_to_pos(size_t line_index , size_t col_index);

void update_text_display(simple_file *file_ptr , bool display_line_no , bool display_file_name , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned end_y);

void render_background();

#endif