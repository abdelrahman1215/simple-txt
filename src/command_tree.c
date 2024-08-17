#include "../headers/command_tree.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct command_node command_node;

struct command_node{
    command_info info;

    command_node *next_sibling;
    command_node *first_offspring;
};

struct command_tree{
    command_node *root_nodes[52];
};

command_node *create_command_node(const char *command , short token_no , command_func *command_exec){
    if(command == NULL/* || command_exec == NULL*/) return NULL;
    if(command[0] == '\000') return NULL;

    command_node *ret = malloc(sizeof(command_node));
    if(ret == NULL) return NULL;

    size_t len = strlen(command);
    ret -> info.command = calloc(len + 1 , sizeof(char));
    if(ret -> info.command == NULL){
        free(ret);

        return NULL;
    }

    strncpy(ret -> info.command , command , len);

    ret -> info.command_len = len;
    ret -> info.command_exec = command_exec;
    ret -> info.token_no = token_no;

    ret -> first_offspring = NULL;
    ret -> next_sibling = NULL;

    return ret;
}

command_tree *new_command_tree(){
    command_tree *ret = malloc(sizeof(command_tree));
    if(ret == NULL) return NULL;

    memset(ret -> root_nodes , 0 , 52 * sizeof(command_tree *));

    return ret;
}

void free_subtree(command_node *node_ptr){
    if(node_ptr == NULL) return ;

    free_subtree(node_ptr -> first_offspring);
    free_subtree(node_ptr -> next_sibling);

    free(node_ptr -> info.command);
    free(node_ptr);
}

void destroy_command_tree(command_tree *tree_ptr){
    if(tree_ptr == NULL) return ;

    for(int i = 0 ; i < 52 ; i++){
        if(tree_ptr -> root_nodes[i] == NULL){
            continue;
        }

        free_subtree(tree_ptr -> root_nodes[i]);
    }
    free(tree_ptr);
}

void command_tree_add_node(command_tree *tree_ptr , const char *command , unsigned short token_no , command_func *command_exec){
    if(tree_ptr == NULL || command == NULL/* || command_exec == NULL*/) return ;
    for(size_t i = 0 ; command[i] ; i++){
        if(isalpha(command[i] == 0)) return ;
    }

    command_node *new_node = create_command_node(command , token_no , command_exec);
    if(new_node == NULL) return ;

    command_node **nd = &(tree_ptr -> root_nodes[command[0] - 'A' - ((islower(command[0]) != 0) * 6)]/*to deal with the diference in value between 'Z' and 'a'*/);
    int str_diff = 0;
    size_t min_len;
    while(1){
        if(*nd == NULL){
            *nd = new_node;//done
            return;
        }

        if(new_node -> info.command_len < (*nd) -> info.command_len){
            new_node -> first_offspring = *nd;

            for(command_node **node = nd ; *node != NULL ; node = &((*node) -> next_sibling)){
                if(strncmp((*node) -> info.command , command , new_node -> info.command_len) != 0){
                    new_node -> next_sibling = *node;
                    *node = NULL;

                    break;
                }
            }

            *nd = new_node;//done
            return;
        }

        for( ; (*nd) != NULL ; nd = &((*nd) -> next_sibling)){
            min_len = (*nd) -> info.command_len < new_node -> info.command_len ? (*nd) -> info.command_len : new_node -> info.command_len;
            str_diff = strncmp((*nd) -> info.command , command , min_len);

            if(str_diff == 0){
                if((*nd) -> info.command_len == new_node -> info.command_len){
                    free_subtree(new_node);
                    return;
                }

                break;
            }
        }

        if(*nd == NULL ) continue;
        if(new_node -> info.command_len < (*nd) -> info.command_len) continue;

        nd = &((*nd) -> first_offspring);
    }

    free_subtree(new_node);
}

command_node **_command_tree_find_node_ptr_(command_tree *tree_ptr , const char *command){
    if(tree_ptr == NULL || command == NULL) return NULL;
    for(size_t i = 0 ; command[i] ; i++){
        if(isalpha(command[i] == 0)) return NULL;
    }

    command_node **ret;
    size_t len = strlen(command);

    int str_diff;
    size_t min_len;
    for(ret = &(tree_ptr -> root_nodes[command[0] - 'A']) ; *ret != NULL ; ret = &((*ret) -> first_offspring)){
        //if((*ret) -> info.command_len == len) break;
        //if((*ret) -> info.command_len > len) return NULL;

        while(1){
            min_len = (*ret) -> info.command_len < len ? (*ret) -> info.command_len : len;
            str_diff = strncmp((*ret) -> info.command , command , min_len);

            if(str_diff == 0){
                if((*ret) -> info.command_len == len){
                    return ret;
                }

                break;
            }

            if((*ret) -> next_sibling == NULL){
                break;
            }

            ret = &((*ret) -> next_sibling);
        }

        if((*ret) -> first_offspring == NULL){
            break;
        }
    }


    return NULL;
}

void command_tree_delete_node(command_tree *tree_ptr , const char *command){
    command_node **target_node_ptr = _command_tree_find_node_ptr_(tree_ptr , command);
    if(target_node_ptr == NULL) return ;

    command_node *tmp = *target_node_ptr;

    if((*target_node_ptr) -> next_sibling != NULL){
        (*target_node_ptr) -> next_sibling -> first_offspring = (*target_node_ptr) -> first_offspring;
    }
    
    *target_node_ptr = (*target_node_ptr) -> next_sibling;

    tmp -> first_offspring = NULL;
    tmp -> next_sibling = NULL;

    free_subtree(tmp);
}

command_info command_tree_find_command(command_tree *tree_ptr , const char *command){
    command_node **target_node_ptr = _command_tree_find_node_ptr_(tree_ptr , command);
    if(target_node_ptr == NULL) return (command_info){.command = NULL , .command_exec = NULL , .command_len = 0 , .token_no = 0};

    return (*target_node_ptr) -> info;
}