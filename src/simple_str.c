#include "../headers/simple_str.h"

#include <stdlib.h>
#include <string.h>

struct fat_str{
    char *str;
    size_t strlen;
    size_t actual_size;
};

#define chunck_size 16

simple_str *new_simple_str(char *src){
    simple_str *ret = calloc(1 , sizeof(simple_str));
    if(ret == NULL) return NULL;

    size_t len = 0;
    if(src != NULL){
        len = strnlen(src , 9223372036854775807);
    }

    size_t actual_size = len + (chunck_size - (len % chunck_size));

    ret -> str = calloc(actual_size , sizeof(char));
    if(ret -> str == NULL){
        free(ret);
        return NULL;
    }

    ret -> strlen = len;
    ret -> actual_size = actual_size;

    if(src != NULL){
        strncpy(ret -> str , src , ret -> strlen);
    }

    return ret;
}

void simple_str_add(simple_str *str_ptr , char *source , size_t index){
    if(str_ptr == NULL) return ;
    if(index > str_ptr -> strlen) return ;
    if(source == NULL) return ;

    size_t len = strnlen(source , 9223372036854775807);

    if(str_ptr -> strlen + len >= str_ptr -> actual_size){
        size_t realloc_size =  str_ptr -> actual_size + (chunck_size * ((len / chunck_size) + 1));

        char *tmp = realloc(str_ptr -> str , realloc_size);
        if(tmp == NULL){
            return;
        }

        str_ptr -> str = tmp;
        str_ptr -> actual_size = realloc_size;
        memset(str_ptr -> str + str_ptr -> strlen , '\000' , str_ptr -> actual_size - str_ptr -> strlen);
    }

    char *target = str_ptr -> str + index;
    memmove(target + len , target , str_ptr -> strlen - index);
    strncpy(target , source , len);

    str_ptr -> strlen += len;

    char *new_str;
}

void simple_str_delete(simple_str *str_ptr , size_t index , size_t count){
    if(str_ptr == NULL) return ;
    if(index >= str_ptr -> strlen || index + count > str_ptr -> strlen) return;

    char *target = str_ptr -> str + index;
    strncpy(target , target + count , str_ptr -> strlen - (index + count));

    str_ptr -> strlen -= count;

    target = str_ptr -> str + str_ptr -> strlen;
    memset(target , '\000' , count);

    if((str_ptr -> actual_size - str_ptr -> strlen) / chunck_size >= 2){
        size_t realloc_size = str_ptr -> actual_size - chunck_size;
        char *tmp = realloc(str_ptr -> str , realloc_size);
        if(tmp == NULL){
            return ;
        }

        str_ptr -> str = tmp;
        str_ptr -> actual_size = realloc_size;
    }
}

void destroy_simple_str(simple_str *str_ptr){
    if(str_ptr == NULL) return ;

    free(str_ptr -> str);
    free(str_ptr);
}

size_t simple_str_get_strlen(simple_str *str_ptr){
    if(str_ptr == NULL) return 0;

    return str_ptr -> strlen;
}

char *simple_str_get_string(simple_str *str_ptr){
    if(str_ptr == NULL) return NULL;

    char *dest = calloc(str_ptr -> strlen + 2 , sizeof(char));
    if(dest == NULL) return NULL;

    return strncpy(dest , str_ptr -> str , str_ptr -> strlen);
}

void simple_str_copy_str(simple_str *src , size_t copy_start , char *dest , size_t dest_size){
    if(src == NULL || dest == NULL || dest_size == 0) return ;

    size_t src_len = src -> strlen;
    if(copy_start >= src -> strlen) return ;

    size_t copy_len = dest_size - 1;
    
    if(copy_len + copy_start > src_len) copy_len = src_len - copy_start;

    strncpy(dest , src -> str + copy_start , copy_len);
}