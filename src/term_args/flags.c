#include "../../headers/simple_globals.h"

#include "stdlib.h"

//a flag starts with - or --
//write the flags above every flag function
//flag functions work by changing the values of global variables

//-v --version
void version(){
    printf("%s\n" , VERSION);

    exit(0);
}