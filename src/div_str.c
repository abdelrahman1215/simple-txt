#include "../headers/div_str.h"

#include <stdlib.h>
#include <string.h>

char **div_str_into_lines(char *str , size_t *get_line_no){
    if(str == NULL || get_line_no == NULL) return NULL;

    *get_line_no = 1;
    size_t len = strlen(str);
    char *copy = calloc(len + 1 , sizeof(char));
    strncpy(copy , str , len);

    for(size_t i = 0 ; i < len ; i++){
        if(copy[i] == '\n'){
            copy[i] = '\000';
            (*get_line_no) ++;
        }
    }

    char **ret = malloc(sizeof(char *) * (*get_line_no));
    if(ret == NULL){
        free(copy);
        *get_line_no = 0;
        
        return NULL;
    }

    size_t line_start = 0 , lines = 0;

    for(size_t i = 0 ; i < len ; i++){
        if(copy[i] == '\000'){
            ret[lines ++] = copy + line_start;
            line_start = i + 1;
        }
    }

    ret[lines] = copy + line_start;

    return ret;
}