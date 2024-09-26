#ifndef GET_STR_H
#define GET_STR_H

#include <pdcurses.h>

typedef struct autocomp_info {
    size_t replace_start , replace_end;
    char *replacement;
} autocomp_info;

typedef autocomp_info (autocomp_func) (char *input , size_t current_pos);

char *get_str(WINDOW *inp_window , autocomp_func autocomplete , unsigned int background_pair , unsigned int text_pair , unsigned int start_x , unsigned int end_x , unsigned int start_y , unsigned int end_y);

#endif