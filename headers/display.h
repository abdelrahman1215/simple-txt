#ifndef DISPLAY_H
#define DISPLAY_H

#include "simple_file.h"

#include <pdcurses.h>
#include <stdbool.h>

typedef struct text_display_info text_display_info;

void display_messages();

void init_color_pairs();

text_display_info *new_text_disp_info();

void update_text_display(simple_file *file_ptr , text_display_info *save_info , WINDOW *disp_window , unsigned int background_pair , unsigned int text_pair , unsigned int line_highlight_pair , unsigned int side_strip_pair , unsigned int side_strip_highlight_pair , unsigned int least_v_dist , unsigned int least_h_dist , bool display_line_no , bool display_file_name  , bool highlight_current_line , bool Scroll , bool disp_cursor , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned end_y);

void render_background(WINDOW *disp_window , unsigned int start_x , unsigned int start_y , unsigned int end_x , unsigned int end_y , int color_pair);

void update_lower_strip(WINDOW *disp_window);

#endif