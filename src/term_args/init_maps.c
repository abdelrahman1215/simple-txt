#include "../../c_datastructures/headers/hashmap.h"
#include "stddef.h"

hashmap *flags_map = NULL , *options_map = NULL;

typedef void (opt_func) (char **tokens);
typedef void (flag_func) ();

typedef struct option_info{
    unsigned short token_no;

    opt_func *option_exec;
}option_info;

typedef struct flag_info{
    flag_func *flag_exec;
}flag_info;

char *option_names[] = {};
option_info options[] = {};

char *flag_names[] = {};
flag_info flags[] = {};

void init_flag_map(){
    if(flags_map != NULL) return ;

    if(sizeof(flags) == 0) return ;//till flags are made

    size_t size = sizeof(flags) / sizeof(flag_info);
    flags_map = new_hashmap(size , NULL , true);

    for(size_t i = 0 ; i < size ; i++){
        hashmap_add_entry(flag_names[i] , flags + i , sizeof(flag_info) , NULL , flags_map);
    }
}

void init_option_map(){
    if(options_map != NULL) return ;

    if(sizeof(options) == 0) return ;//till options are made

    size_t size = sizeof(options) / sizeof(option_info);
    options_map = new_hashmap(size , NULL , true);

    for(size_t i = 0 ; i < size ; i++){
        hashmap_add_entry(option_names[i] , options + i , sizeof(option_info) , NULL , flags_map);
    }
}