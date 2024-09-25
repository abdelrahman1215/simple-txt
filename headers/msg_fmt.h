#ifndef MSG_FMT
#define MSG_FMT

#include "parse_term_args.h"
#include "parse_commands.h"
#include "simple_file.h"

char *dequeue_msg();

void loading_msg(const char *file_name , loading_err error);
void wrote_into_file_msg(const char *file_name);

char *dequeue_error();

void loading_error(const char *file_name ,  loading_err error_type);
void parsing_error(const char *command ,  parsing_errors error_type);
void term_arg_error(const char *arg ,  parsing_term_errors error_type);

#endif