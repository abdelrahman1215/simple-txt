#include "../c_datastructures/headers/linked_list.h"
#include "../headers/error_format.h"

#include <string.h>
#include <stdio.h>

linked_list *error_queue = NULL;

void init_error_queue(){
    if(error_queue != NULL) return ;

    error_queue = new_linked_list();
}

void enqueue_error(char *error_msg){
    if(error_msg == NULL) return ;
    if(error_msg[0] == '\000') return ;

    if(error_queue == NULL) init_error_queue();

    size_t len = strlen(error_msg);
    linked_list_add_node(error_msg , len + 1 , NULL , error_queue);
}

char *dequeue_error(){
    if(error_queue == NULL) return NULL;

    node *target_node = linked_list_get_first_node(error_queue);
    if(target_node == NULL) return NULL;

    char *ret = linked_list_get_obj(target_node);
    
    linked_list_delete_node(0 , error_queue);

    return ret;
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

    enqueue_error(error);
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

    enqueue_error(error);
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

    enqueue_error(error);
}