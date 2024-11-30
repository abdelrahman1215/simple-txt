#include "../../c_datastructures/headers/linked_list.h"
#include "../../headers/msg_fmt.h"
#include"msg_queue.c"

#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define DEF_MSG_LEN 1024

void custom_msg(msg_type type , char *msg){
    if(msg == NULL) return ;
    if(*msg == '\000') return ;

    enqueue_msg(type , msg);
}

void loading_msg(const char *file_name , loading_err error){
    if(file_name == NULL) return ;
    if(file_name[0] == '\000') return ;

    char msg[DEF_MSG_LEN + 1] = {'\000'};
    switch(error){
        case File_Not_Found :
            if(strcmp(file_name , "Untitled") != 0){
                sprintf(msg , "Created new buffer : \"%s\"" , file_name);
            }
            
            break;

        default : return ;
    }

    enqueue_msg(Message , msg);
}

void wrote_into_file_msg(const char *file_name){
    if(file_name == NULL) return ;
    if(file_name[0] == '\000') return ;

    char msg[DEF_MSG_LEN + 1] = {'\000'};

    sprintf(msg , "Wrote changes into \"%s\"." , file_name);
    enqueue_msg(Message , msg);
}

void loading_error(const char *file_name ,  loading_err error_type){
    if(file_name == NULL) return ;
    if(file_name[0] == '\000') return ;

    char error[DEF_MSG_LEN + 1] = {'\000'};
    switch(error_type){
        case Invalid_File_Name :
            sprintf(error , " \"%s\" : Invalid file name" , file_name);
            
            break;

        case Invalid_File_Type :
            sprintf(error , " \"%s\" : Invalid file type" , file_name);
            
            break;

        case File_Not_Found :
            loading_msg(file_name , error_type);

            return;

        default : return ;
    }

    enqueue_msg(Error , error);
}

void parsing_error(const char *command ,  parsing_errors error_type){
    if(command == NULL) return ;
    if(command[0] == '\000') return ;

    char error[DEF_MSG_LEN + 1] = {'\000'};

    switch(error_type){
        case Command_Not_Found :
            sprintf(error , "Unknown command : \"%s\"" , command);

            break;

        case Not_Enough_Args :
           sprintf(error , "Too few arguments for command : \"%s\"" , command);

            break;
            
        default : return ;
    }

    enqueue_msg(Error , error);
}

void term_arg_error(const char *arg ,  parsing_term_errors error_type){
    if(arg == NULL) return ;
    if(arg[0] == '\000') return ;

    char error[DEF_MSG_LEN + 1] = {'\000'};

    switch(error_type){
        case Too_Few_Args :
            sprintf(error , "Too few arguments for option \"%s\"" , arg);
            enqueue_msg(Error , error);

            break;

        case Too_Many_Files : 
            sprintf(error , "Too many files , only one was loaded");
            enqueue_msg(Warning , error);

            break;
            
        default : return ;
    }

}

void command_msg(msg_type type , char *command_name , char *msg , char *arg){
    if(command_name == NULL || msg == NULL) return ;

    char message[DEF_MSG_LEN + 1] = {'\000'};

    if(arg == NULL) sprintf(message , "%s : %s" , command_name , msg);
    else sprintf(message , "%s : %s \"%s\"" , command_name , msg , arg);

    enqueue_msg(type , message);
}