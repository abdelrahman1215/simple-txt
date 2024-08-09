#ifndef ERRORS_H
#define ERRORS_H

#include "../headers/simple_file.h"

char *dequeue_error();

void loading_error(const char *file_name ,  loading_err error_type);

#endif