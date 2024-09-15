#include "display_txt.c"

#include <string.h>

void render_background(){
    if(stdscr == NULL) return ;
    attron(COLOR_PAIR(BACKGROUND));

    for(int i = 0 ; i < Screen_Height ; i++){
        mvhline(i , 0 , 0 , Screen_Width);
    }

    attroff(COLOR_PAIR(BACKGROUND));
}

void init_color_pairs(){
    if(stdscr == NULL) return ;

    init_pair(TEXT , Text_Color , Background_Color);
    init_pair(TITLE , Title_Color , Background_Color);
    init_pair(SIDE_STRIPS , Outline_Color , Background_Color);
    init_pair(BACKGROUND , Background_Color , Background_Color);
    init_pair(LINE_HIGHLIGHT , Text_Color , Line_Highlight_Color);
    init_pair(SIDE_STRIP_HIGHLIGHT , Outline_Color , Line_Highlight_Color);
}