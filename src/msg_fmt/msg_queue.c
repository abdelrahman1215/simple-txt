#include "../../c_datastructures/headers/linked_list.h"
#include "../../headers/msg_fmt.h"

#include <string.h>

linked_list *msg_queue = NULL;

struct msg{
    msg_type type;
    char *msg;
};

void init_msg_queue(){
    if(msg_queue != NULL) return ;

    msg_queue = new_linked_list();
}

void enqueue_msg(msg_type type , char *msg){
    if(msg == NULL) return ;
    if(msg[0] == '\000') return ;

    if(msg_queue == NULL) init_msg_queue();

    size_t len = strlen(msg);
    struct msg message = {.type = type , .msg = calloc(len + 1 , sizeof(char))};    
    if(message.msg == NULL) return ;
    memcpy(message.msg , msg , len);

    linked_list_add_node(&message , sizeof(struct msg) , NULL , msg_queue);
}

char *dequeue_msg(msg_type *get_type){
    if(msg_queue == NULL) return NULL;

    node *target_node = linked_list_get_first_node(msg_queue);
    if(target_node == NULL) return NULL;

    struct msg *target = linked_list_get_obj(target_node);
    
    *get_type = target -> type;

    size_t len = strlen(target -> msg);
    char *ret = calloc(len + 1 , sizeof(char));

    if(ret == NULL) return NULL;
    
    memcpy(ret , target -> msg , len);



    linked_list_delete_node(0 , msg_queue);

    return ret;
}