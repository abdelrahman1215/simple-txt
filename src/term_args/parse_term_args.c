#include "open_file.c"

void parse_term_args(int argc , char **argv){
    //remove this if statement if flags or options are made in the future
    if(argc > 2) return ;//TODO : make a function to format term_arg errors

    if(argc == 1){
        open_file("new_file");
    }else{
        open_file(argv[1]);
    }
}