#include "../../c_datastructures/headers/linked_list.h"
#include "../../headers/msg_format.h"

#include <string.h>
#include <stdio.h>

linked_list *msg_queue = NULL;

void init_msg_queue(){
    if(msg_queue != NULL) return ;

    msg_queue = new_linked_list();
}

void enqueue_msg(char *msg){
    if(msg == NULL) return ;
    if(msg[0] == '\000') return ;

    if(msg_queue == NULL) init_msg_queue();

    size_t len = strlen(msg);
    linked_list_add_node(msg , len + 1 , NULL , msg_queue);
}

char *dequeue_msg(){
    if(msg_queue == NULL) return NULL;

    node *target_node = linked_list_get_first_node(msg_queue);
    if(target_node == NULL) return NULL;

    char *ret = linked_list_get_obj(target_node);
    
    linked_list_delete_node(0 , msg_queue);

    return ret;
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

    enqueue_msg(msg);
}

void wrote_into_file_msg(const char *file_name){
    if(file_name == NULL) return ;
    if(file_name[0] == '\000') return ;

    char msg[1024];
    memset(msg , 0 , 1024);

    sprintf(msg , "wrote the changes into \"%s\"" , file_name);
}