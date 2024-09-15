#ifndef SIMPLE_GLOBALS
#define SIMPLE_GLOBALS

#include "simple_file.h"

#include <stddef.h>
#include <pdcurses.h>

#define SIMPLE_GLOBAL __attribute__((__common__))

#define DEFAULT_TITLE_COLOR 0x6F
#define DEFAULT_OUTLINE_COLOR 0xED
#define DEFAULT_BACKGROUND_COLOR 0xEA
#define DEFAULT_TEXT_COLOR COLOR_WHITE
#define DEFAULT_LINE_HIGHLIGHT_COLOR 0xEB

#define TEXT 1
#define SIDE_STRIPS 2
#define BACKGROUND 3
#define LINE_HIGHLIGHT 4
#define SIDE_STRIP_HIGHLIGHT 5
#define TITLE 6

SIMPLE_GLOBAL simple_file *Current_File;
SIMPLE_GLOBAL unsigned int Txt_Disp_Start_X , Txt_Disp_Start_Y , Txt_Disp_End_X , Txt_Disp_End_Y;
SIMPLE_GLOBAL unsigned int Least_H_Distance , Least_V_Distance;
SIMPLE_GLOBAL unsigned int Screen_Width , Screen_Height;

#endif