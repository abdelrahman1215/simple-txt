#ifndef FILE_STRUCT
#define FILE_STRUCT

#include "../../c_datastructures/headers/dynamic_array.h"
#include "../../c_datastructures/headers/linked_list.h"
#include "../../headers/token_graph.h"
#include <stddef.h>

struct file_struct{
    char *file_name;
    bool changes_saved;

    dynamic_array *lines;
    size_t line , column;
    size_t max_reached_column;

    linked_list *changes_stack;
    linked_list *undone_stack;

    token_graph *tk_graph;
};

#endif