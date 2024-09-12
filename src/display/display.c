#include "display_txt.c"
#include "file_pos.c"

void update_background(){
    attron(COLOR_PAIR(BACKGROUND));

    for(int i = 0 ; i < Screen_Height ; i++){
        mvhline(i , 0 , 0 , Screen_Width);
    }

    attroff(COLOR_PAIR(BACKGROUND));
}

void init_color_pairs(){
    init_pair(TEXT , DEFAULT_TEXT_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(SIDE_STRIPS , DEFAULT_OUTLINE_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(BACKGROUND , DEFAULT_BACKGROUND_COLOR , DEFAULT_BACKGROUND_COLOR );
    init_pair(LINE_HIGHLIGHT , DEFAULT_TEXT_COLOR , DEFAULT_LINE_HIGHLIGHT_COLOR );
    init_pair(LINE_HIGHLIGHT_BACKGROUND , DEFAULT_LINE_HIGHLIGHT_COLOR , DEFAULT_LINE_HIGHLIGHT_COLOR );
}