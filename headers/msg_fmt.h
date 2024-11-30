#ifndef MSG_FMT
#define MSG_FMT

#include "parse_term_args.h"
#include "parse_commands.h"
#include "simple_file.h"

typedef enum msg_type {Error = 1 , Warning = 2 , Message = 3} msg_type;

char *dequeue_msg(msg_type *get_type);
size_t get_msg_no();

void custom_msg(msg_type type , char *msg);
void loading_msg(const char *file_name , loading_err error);
void wrote_into_file_msg(const char *file_name);
void command_msg(msg_type type , char *command_name , char *msg , char *arg);

void loading_error(const char *file_name ,  loading_err error_type);
void parsing_error(const char *command ,  parsing_errors error_type);
void term_arg_error(const char *arg ,  parsing_term_errors error_type);

#endif