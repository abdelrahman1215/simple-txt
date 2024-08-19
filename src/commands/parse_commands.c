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

        if(info.command == NULL) return;//TODO : make a functions that formats and pushes commands errors

        if(info.token_no > 0){
            if(token_no - i < info.token_no) return ;//error msg here

            args = malloc(info.token_no * sizeof(char *));
            if(args == NULL) return ;

            for(unsigned short j = 0 ; j < info.token_no ; j++){
                args[j] = tokens[i + j + 1];
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

    //char *check[tokens_no];
    //for(int i = 0 ; i < tokens_no ; i++){
    //    check[i] = tokens[i];
    //}
    run_commands(tokens , tokens_no);

    free(tokens);
    free(copy);
}