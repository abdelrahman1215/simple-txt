#ifndef SIMPLE_FILE
#define SIMPLE_FILE

#include <stdbool.h>
#include <stddef.h>

typedef enum loading_err {Ok = 0 , Is_Dir = 1 , File_Not_Found = 2 , Alloc_Err = 3 , Invalid_Arg = 4 , Invalid_File_Name = 5} loading_err;

typedef struct file_struct simple_file;

bool is_dir(const char *file_name);

simple_file *load_file(const char *file_name , bool create_if_not_found , loading_err *get_err);
simple_file *load_from_str(const char *src , loading_err *get_err);
void save_file(simple_file *file_ptr);
void destroy_simple_file(simple_file *file_ptr);

const char *simple_file_get_name(simple_file *file_ptr);
size_t simple_file_get_line_no(simple_file *file_ptr);
char *simple_file_get_line(simple_file *file_ptr , size_t index);
void simple_file_copy_line(simple_file *file_ptr , size_t index , size_t start_pos , char *dest , size_t dest_size);
size_t simple_file_get_line_len(simple_file *file_ptr , size_t index);
bool simple_file_changes_saved(simple_file *file_ptr);

void simple_file_add(simple_file *file_ptr , size_t line_index , size_t pos_index , char *source);
void simple_file_delete(simple_file *file_ptr , size_t line_index , size_t start_pos , size_t count);
void simple_file_delete_from_to(simple_file *file_ptr , size_t start_line , size_t start_pos , size_t end_line , size_t end_pos);
void simple_file_delete_lines(simple_file *file_ptr , size_t line_index , size_t line_count);

void simple_file_undo(simple_file *file_ptr);
void simple_file_redo(simple_file *file_ptr);

void simple_file_move_ncols_right(simple_file *file_ptr , size_t cols);
void simple_file_move_ncols_left(simple_file *file_ptr , size_t cols);
void simple_file_move_nlines_down(simple_file *file_ptr , size_t lines);
void simple_file_move_nlines_up(simple_file *file_ptr , size_t lines);
void simple_file_move_to_xy(simple_file *file_ptr , size_t line_index , size_t column_index);

size_t simple_file_get_curr_line(simple_file *file_ptr);
size_t simple_file_get_curr_column(simple_file *file_ptr);

#endif