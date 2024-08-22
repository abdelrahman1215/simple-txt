#include "open_file.c"
#include "init_maps.c"

void parse_term_args(int argc , char **argv){
    //remove this if statement if flags or options are made in the future
    if(argc > 2) return ;

    if(argc == 1){
        open_file("new_file");
    }else{
        entry *op_entry = NULL;
        option_info *op_inf = NULL;

        entry *flag_entry = NULL;
        flag_info *flag_inf = NULL;
        for(int i = 1 ; i < argc ; i++){
            if(argv[i][0] == '-'){
                flag_entry = hashmap_lookup_entry(argv[i] , flags_map);
                flag_inf = hashmap_get_obj(flag_entry);

                flag_inf -> flag_exec();
                free(flag_inf);

                continue;
            }

            op_entry = hashmap_lookup_entry(argv[i] , options_map);
            op_inf = hashmap_get_obj(op_entry);

            if(op_inf != NULL){
                if(op_inf -> token_no > 0){
                    if(argc - i < op_inf -> token_no){
                        return ;//TODO : add an error for too few args
                    }

                    char *args[op_inf -> token_no];
                    for(int j = i + 1  , k = 0 ; j < op_inf -> token_no ; j++ , k++){
                        args[k] = argv[j];
                    }

                    op_inf -> option_exec(args);
                }else{
                    op_inf -> option_exec(NULL);
                }

                free(op_inf);

                continue;
            }

            open_file(argv[i]);
            return;
        }
    }
}