#ifndef SIMPLE_STR

#define SIMPLE_STR

#include <stddef.h>

typedef struct fat_str simple_str;

/// @brief makes a new simple str
/// @param init_len the initial length of the string
/// @param src a str to copy from , set to NULL if you want an empty string
/// @return a dynamiclly allocated simple str
simple_str *new_simple_str(char *src);
void destroy_simple_str(simple_str *str_ptr);

void simple_str_add(simple_str *str_ptr , char *source , size_t index);
void simple_str_delete(simple_str *str_ptr , size_t index , size_t count);


char *simple_str_get_string(simple_str *str_ptr);
size_t simple_str_get_strlen(simple_str *str_ptr);


#endif