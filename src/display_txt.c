#include "../headers/simple_globals.h"
#include "../headers/simple_file.h"

#include <pdcurses.h>
#include <stdlib.h>
#include <string.h>

void update_text_display(){
    if(stdscr == NULL) return ;

    if(Current_File == NULL) return ;

    unsigned int row_no = Txt_Disp_End_Y - Txt_Disp_Start_Y;
    unsigned int max_len = (Txt_Disp_End_X - Txt_Disp_Start_X);
    
    char rows[row_no][max_len + 1];
    for(unsigned int i = 0 ; i < row_no ; i++){
        memset(rows[i] , 0 , max_len + 1);
    }

    char *tmp = NULL;
    size_t len;
    unsigned int used_rows = 0;
    for(unsigned int i = 0 ; i < row_no ; i++ , used_rows++){
        len = simple_file_get_line_len(Current_File , i + Txt_Start_Line);
        if(Txt_Disp_Indent >= len) continue;
        
        tmp = simple_file_get_line(Current_File , i + Txt_Start_Line);
        if(tmp == NULL) break;

        strncpy(rows[i] , tmp + Txt_Disp_Indent , max_len);

        free(tmp);
    }

    for(unsigned int i = used_rows ; i < row_no ; i++){
        rows[i][0] = '~';
    }

    curs_set(0);

    for(unsigned int i = 0 ; i < row_no ; i++){
        mvprintw(Txt_Disp_Start_Y + i , Txt_Disp_Start_X , "%s" , rows[i]);
    }

    curs_set(1);

    move(Txt_Disp_Start_Y , Txt_Disp_Start_X);
}