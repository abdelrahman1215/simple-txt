#include "display_txt.c"
#include "file_pos.c"

void update_background(){
    attron(COLOR_PAIR(BACKGROUND));

    for(int i = 0 ; i < Screen_Height ; i++){
        mvhline(i , 0 , 0 , Screen_Width);
    }

    attroff(COLOR_PAIR(BACKGROUND));
}