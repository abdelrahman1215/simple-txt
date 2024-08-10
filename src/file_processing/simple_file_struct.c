#include "../../c_datastructures/headers/dynamic_array.h"
#include "../../c_datastructures/headers/linked_list.h"

struct file_struct{
    char *file_name;

    dynamic_array *lines;

    linked_list *changes_stack;
    linked_list *undone_stack;
};