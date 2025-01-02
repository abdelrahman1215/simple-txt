#ifndef OPEN_FILES_H
#define OPEN_FILES_H

#include "curses_header.h"


void open_file(const char *file_name);
void open_dir(const char *dir_name , WINDOW *disp_window , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned int end_y);

#endif