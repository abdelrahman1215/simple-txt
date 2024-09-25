#include "../../c_datastructures/headers/linked_list.h"
#include "../../headers/msg_fmt.h"
#include"msg_queue.c"

#include <string.h>
#include <stdio.h>

void custom_msg(msg_type type , char *msg){
    if(msg == NULL) return ;
    if(*msg == '\000') return ;

    enqueue_msg(type , msg);
}

void loading_msg(const char *file_name , loading_err error){
    if(file_name == NULL) return ;
    if(file_name[0] == '\000') return ;

    char msg[1024];
    memset(msg , 0 , 1024);
    switch(error){
        case File_Not_Found :
            sprintf(msg , "created new file \"%s\"" , file_name);
            
            break;

        default : return ;
    }

    enqueue_msg(Message , msg);
}

void wrote_into_file_msg(const char *file_name){
    if(file_name == NULL) return ;
    if(file_name[0] == '\000') return ;

    char msg[1024];
    memset(msg , 0 , 1024);

    sprintf(msg , "wrote the changes into \"%s\"" , file_name);
}

void loading_error(const char *file_name ,  loading_err error_type){
    if(file_name == NULL) return ;
    if(file_name[0] == '\000') return ;

    char error[1024] = "loading error : ";
    strncpy(error , "loading error : " , strlen("loading error : "));

    strncat(error , file_name , strlen(file_name));

    switch(error_type){
        case Is_Dir :
            strncat(error , " is a directory. " , strlen(" is a directory."));
            break;

        default : return ;
    }

    enqueue_msg(Error , error);
}

void parsing_error(const char *command ,  parsing_errors error_type){
    if(command == NULL) return ;
    if(command[0] == '\000') return ;

    char error[1024] = "parsing error : ";
    strncpy(error , "parsing error : " , strlen("parsing error : "));

    switch(error_type){
        case Command_Not_Found :
            strncat(error , "the command \" " , strlen("the command \""));
            strncat(error , command , strlen(command));
            strncat(error , "\" was not found. " , strlen("\" was not found."));

            break;

        case Not_Enough_Args :
            strncat(error , "too few arguments for the command \" " , strlen("too few arguments for the command \""));
            strncat(error , command , strlen(command));
            strncat(error , "\". " , strlen("\"."));

            break;
            
        default : return ;
    }

    enqueue_msg(Error , error);
}

void term_arg_error(const char *arg ,  parsing_term_errors error_type){
    if(arg == NULL) return ;
    if(arg[0] == '\000') return ;

    char error[1024];
    memset(error , 0 , 1024 * sizeof(char));

    switch(error_type){
        case Too_Few_Args :
            sprintf(error , "too few arguments for option \"%s\"" , arg);

            break;

        case Too_Many_Files : 
            sprintf(error , "too many files , only one was loaded");

            break;
            
        default : return ;
    }

    enqueue_msg(Error , error);
}