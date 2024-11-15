#include "../../headers/simple_globals.h"
#include "../../headers/manual.h"

#include "stdlib.h"

//a flag starts with - or --
//write the flags above every flag function
//flag functions work by changing the values of global variables

//-v --version
void version(){
    printf("%s\n" , VERSION);

    exit(0);
}

//-h --help
void help_flag(){
    char *manual = get_manual();
    printf("%s\n" , manual);

    exit(0);
}