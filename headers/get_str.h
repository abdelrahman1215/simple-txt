#ifndef GET_STR_H
#define GET_STR_H

#include "curses_header.h"

typedef struct autocomp_info {
    size_t replace_start , replace_end;

    size_t opt_no;
    char **rep_options;
} autocomp_info;

typedef autocomp_info (autocomp_func) (char *input , size_t current_pos , bool get_replacment);

char *get_str(WINDOW *inp_window , autocomp_func autocomplete , unsigned int background_pair , unsigned int text_pair , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned int end_y);

#endif