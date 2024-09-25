#include "../../headers/msg_fmt.h"
#include "init_command_tree.c"

#include <string.h>

void run_commands(char **tokens , unsigned short token_no){
    if(tokens == NULL || token_no == 0) return ;
    for(size_t i = 0 ; i < token_no ; i++){
        if(tokens[i] == NULL) return ;
        if(tokens[i][0] == '\000') return;
    }

    command_info info;
    char **args;
    for(unsigned short i = 0 ; i < token_no ; i++){
        info = command_tree_find_command(Command_Tree , tokens[i]);

        if(info.command == NULL){
            parsing_error(tokens[i] , Command_Not_Found);

            return ;
        }

        if(info.token_no > 0){
            if(token_no - (i + 1) < info.token_no){
                parsing_error(tokens[i] , Not_Enough_Args);
                
                return ;
            }

            args = malloc(info.token_no * sizeof(char *));
            if(args == NULL) return ;

            for(unsigned short j = i + 1  , k = 0 ; j < info.token_no ; j++ , k++){
                args[k] = tokens[j];
            }


            i += info.token_no;
        }else{
            args = NULL;
        }

        info.command_exec(args);

        if(info.token_no > 0) free(args);
    }
}

void parse_command(char *input){
    if(input == NULL) return ;
    if(input[0] == '\000') return ;

    size_t len = strlen(input);
    char *copy = calloc(strlen(input) + 1 , sizeof(char));
    strncpy(copy , input , len);

    if(Command_Tree == NULL) init_command_tree();

    char delimeters[] = " ";
    char **tokens = malloc(sizeof(char *));
    if(tokens == NULL){
        free(copy);

        return;
    }
    
    char *token = strtok(copy , delimeters);
    unsigned short tokens_no = 0;
    for(char **tmp ; token != NULL ; token = strtok(NULL , delimeters) , tokens_no++){
        size_t size = (tokens_no + 1) * sizeof(char *);
        tmp = realloc(tokens , size);
        if(tmp == NULL){
            free(tokens);
            free(copy);
            return ;
        }

        tokens = tmp;
        tokens[tokens_no] = token;
    }

    run_commands(tokens , tokens_no);

    free(tokens);
    free(copy);
}