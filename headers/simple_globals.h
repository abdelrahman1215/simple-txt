#ifndef SIMPLE_GLOBALS
#define SIMPLE_GLOBALS

#include "simple_file.h"

#define SIMPLE_GLOBAL __attribute__((__common__))

SIMPLE_GLOBAL simple_file *current_file;

#endif