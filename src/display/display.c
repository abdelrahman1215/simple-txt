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

    init_pair(TEXT , DEFAULT_TEXT_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(TITLE , DEFAULT_TITLE_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(SIDE_STRIPS , DEFAULT_OUTLINE_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(BACKGROUND , DEFAULT_BACKGROUND_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(LINE_HIGHLIGHT , DEFAULT_TEXT_COLOR , DEFAULT_LINE_HIGHLIGHT_COLOR );

}