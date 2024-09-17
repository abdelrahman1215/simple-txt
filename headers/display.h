#ifndef DISPLAY_H
#define DISPLAY_H

#include "simple_file.h"

#include <stdbool.h>

void init_color_pairs();

void update_text_display(simple_file *file_ptr , bool display_line_no , bool display_file_name , bool highlight_current_line  , bool scroll , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned end_y);

void render_background(unsigned int start_x , unsigned int start_y , unsigned int end_x , unsigned int end_y);

#endif