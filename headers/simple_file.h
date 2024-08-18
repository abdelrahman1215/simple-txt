#ifndef SIMPLE_FILE
#define SIMPLE_FILE

#include <stddef.h>

typedef enum loading_err {OK = 0 , Is_Dir = 1 , File_Not_Found = 2 , Alloc_Err = 3 , Invalid_Arg = 4} loading_err;

typedef struct file_struct simple_file;

bool is_dir(const char *file_name);

simple_file *load_file(const char *file_name , loading_err *get_err);
void save_file(simple_file *file_ptr);
void destroy_simple_file(simple_file *file_ptr);

size_t simple_file_get_line_no(simple_file *file_ptr);
char *simple_file_get_line(simple_file *file_ptr , size_t index);
size_t simple_file_get_line_len(simple_file *file_ptr , size_t index);
bool simple_file_changes_saved(simple_file *file_ptr);

void simple_file_add(simple_file *file_ptr , size_t line_index , size_t pos_index , char *source);
void simple_file_delete(simple_file *file_ptr , size_t line_index , size_t start_pos , size_t count);
void simple_file_delete_from_to(simple_file *file_ptr , size_t start_line , size_t start_pos , size_t end_line , size_t end_pos);
void simple_file_delete_lines(simple_file *file_ptr , size_t line_index , size_t line_count);

void simple_file_undo(simple_file *file_ptr);
void simple_file_redo(simple_file *file_ptr);

#endif