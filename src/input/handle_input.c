#include "../../headers/simple_globals.h"
#include "../../headers/simple_file.h"
#include "../../headers/input.h"

#include "normal_mode.c"
#include "insert_mode.c"

#include <pdcurses.h>

void handle_input(){
    flushinp();
    int ch;

    do{
        ch = getch();
    }while(ch == ERR);

    switch(Current_Mode){
        case Normal_Mode:
            normal_mode(ch);
            break;

        case Insert_Mode:
            insert_mode(ch);
            break;
    }
}