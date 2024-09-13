#include "display_txt.c"
#include "file_pos.c"

#include <string.h>

void render_background(){
    if(stdscr == NULL) return ;
    attron(COLOR_PAIR(BACKGROUND));

    for(int i = 0 ; i < Screen_Height ; i++){
        mvhline(i , 0 , 0 , Screen_Width);
    }

    attroff(COLOR_PAIR(BACKGROUND));
}

void display_file_name(){
    if(Current_File == NULL || Txt_Disp_Start_Y == 0) return ;

    char *file_path;
    {
        const char *tmp = (char *)simple_file_get_name(Current_File);
        size_t path_len = strlen(tmp) + 1;

        file_path = calloc(path_len + 1 , sizeof(char));
        strncpy(file_path , tmp , path_len);
    }

    char *file_name = strtok(file_path , "/\\");
    for(char *tmp = strtok(NULL , "/\\") ; tmp != NULL ; file_name = tmp , tmp = strtok(NULL , "/\\")){}
    free(file_path);

    size_t name_len = strlen(file_name);
    if(name_len + 6 > (Txt_Disp_End_X - Text_Start_X)){
        if((Txt_Disp_End_X - Text_Start_X) < 6){
            return;
        }
        file_name[(Txt_Disp_End_X - Text_Start_X) - 6] = '\000';
    }

    attron(COLOR_PAIR(TITLE));

    curs_set(0);

    mvprintw(Txt_Disp_Start_Y - 1 , Text_Start_X + ((Txt_Disp_End_X - Text_Start_X)/2) - (strlen(file_name)/2) , "%s" , file_name);
    
    curs_set(1);

    attroff(COLOR_PAIR(TITLE));
}

void init_color_pairs(){
    if(stdscr == NULL) return ;

    init_pair(TEXT , DEFAULT_TEXT_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(TITLE , DEFAULT_TITLE_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(SIDE_STRIPS , DEFAULT_OUTLINE_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(BACKGROUND , DEFAULT_BACKGROUND_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(LINE_HIGHLIGHT , DEFAULT_TEXT_COLOR , DEFAULT_LINE_HIGHLIGHT_COLOR );
    init_pair(LINE_HIGHLIGHT_BACKGROUND , DEFAULT_LINE_HIGHLIGHT_COLOR , DEFAULT_LINE_HIGHLIGHT_COLOR );
}