#include "../../c_datastructures/headers/hashmap.h"

typedef void (binding_func) ();

typedef struct binding{
    int key;
    binding_func *exec;
}binding;

hashmap *binding_map = NULL;
binding binding_arr[] = {
};

typedef union binding_key{
    char str[sizeof(int) + 1];
    unsigned int integer;
} binding_key;

void init_binding_map(){
    if(binding_map != NULL) return ;

    size_t binding_no = sizeof(binding_arr) / sizeof(binding);
    if(binding_no == 0) return ;

    binding_map = new_hashmap(binding_no , NULL , true);
    if(binding_map == NULL) return ;

    binding_key key;

    for(size_t i = 0 ; i < binding_no ; i++){
        key.integer = binding_arr[i].key;
        
        for(int j = 0 ; j < sizeof(int) ; j++){
            if(key.str[j] == '\000'){
                key.str[j] = ' ';
            }
        }

        key.str[4] = '\000';

        hashmap_add_entry(key.str , binding_arr + i , sizeof(binding) , NULL , binding_map);
    }
}

int exec_binding(int input){
    if(binding_map == NULL) init_binding_map();

    binding_key key = {.integer = input};
    for(size_t i = 0 ; i < sizeof(int) ; i++){
        if(key.str[i] == '\000') key.str[i] = ' ';
    }

    key.str[4] = '\000';

    entry *target_entry = hashmap_lookup_entry(key.str , binding_map);
    if(target_entry == NULL) return 1;

    binding *target = hashmap_get_obj(target_entry);

    target -> exec();

    free(target);

    return 0;
}