#include "display_txt.c"

#include <string.h>

void render_background(unsigned int start_x , unsigned int start_y , unsigned int end_x , unsigned int end_y){
    if(stdscr == NULL) return ;
    attron(COLOR_PAIR(BACKGROUND));

    for(unsigned int i = 0 ; i < (end_y - start_y) ; i++){
        mvhline(start_y + i , start_x , 0 , end_x - start_x);
    }

    attroff(COLOR_PAIR(BACKGROUND));
}

void init_color_pairs(){
    if(stdscr == NULL) return ;

    init_pair(TEXT , Text_Color , Background_Color);
    init_pair(TITLE , Title_Color , Background_Color);
    init_pair(SIDE_STRIPS , Outline_Color , Outline_Background_Color);
    init_pair(BACKGROUND , Background_Color , Background_Color);
    init_pair(LINE_HIGHLIGHT , Text_Color , Line_Highlight_Color);
    init_pair(SIDE_STRIP_HIGHLIGHT , Outline_Highlight_Color , Outline_Background_Color);
}