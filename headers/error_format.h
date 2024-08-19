#ifndef ERRORS_H
#define ERRORS_H

#include "parse_commands.h"
#include "simple_file.h"

char *dequeue_error();

void loading_error(const char *file_name ,  loading_err error_type);
void parsing_error(const char *command ,  parsing_errors error_type);

#endif