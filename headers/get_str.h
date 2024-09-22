#ifndef GET_STR_H
#define GET_STR_H

#include <pdcurses.h>

char *get_str(WINDOW *inp_window , unsigned int background_pair , unsigned int text_pair , bool warp_text , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned int end_y);

#endif