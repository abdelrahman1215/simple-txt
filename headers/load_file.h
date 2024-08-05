#ifndef LOAD_FILE
#define LOAD_FILE
#include "../c_datastructures/headers/dynamic_array.h"
#include <stddef.h>

typedef enum loading_err {OK = 0 , Is_Dir = 1 , File_Not_Found = 2 , Alloc_Err = 3 , Invalid_Arg = 4} loading_err;

typedef struct file_struct simple_file;

simple_file *load_file(const char *file_name , loading_err *get_err);
void save_file(simple_file *file_ptr);
void destroy_file(simple_file *file_ptr);

size_t simple_file_get_line_no(simple_file *file_ptr);
char *simple_file_get_line(simple_file *file_ptr , size_t index);
dynamic_array *simple_file_get_content(simple_file *file_str);

#endif