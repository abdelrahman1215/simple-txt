#ifndef MSG_FMT
#define MSG_FMT

#include "../headers/simple_file.h"

char *dequeue_msg();

void loading_msg(const char *file_name , loading_err error);
void wrote_into_file_msg(const char *file_name);

#endif