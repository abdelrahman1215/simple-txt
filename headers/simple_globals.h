#ifndef SIMPLE_GLOBALS
#define SIMPLE_GLOBALS

#include "simple_file.h"

#include <stddef.h>
#include <pdcurses.h>

#define SIMPLE_GLOBAL __attribute__((__common__))

typedef enum editor_mode {Normal_Mode = 1 , Insert_Mode = 2} editor_mode;

SIMPLE_GLOBAL editor_mode Current_Mode;

#define DEFAULT_TEXT_COLOR 0xE6
#define DEFAULT_TITLE_COLOR 0xCA
#define DEFAULT_OUTLINE_COLOR 0xED
#define DEFAULT_BACKGROUND_COLOR 0xE9
#define DEFAULT_LINE_HIGHLIGHT_COLOR 0xEB
#define DEFAULT_LOWER_STRIP_TEXT_COLOR 0xA6
#define DEFAULT_OUTLINE_HIGHLIGHT_COLOR 0xD7
#define DEFAULT_OUTLINE_BACKGROUND_COLOR 0xE9
#define DEFAULT_LOWER_STRIP_BACKGROUND_COLOR 0xE9

SIMPLE_GLOBAL unsigned short Text_Color;
SIMPLE_GLOBAL unsigned short Title_Color;
SIMPLE_GLOBAL unsigned short Outline_Color;
SIMPLE_GLOBAL unsigned short Background_Color;
SIMPLE_GLOBAL unsigned short Line_Highlight_Color;
SIMPLE_GLOBAL unsigned short Lower_Strip_Text_Color;
SIMPLE_GLOBAL unsigned short Outline_Highlight_Color;
SIMPLE_GLOBAL unsigned short Lower_Strip_Background_Color;

#define TEXT 1
#define SIDE_STRIPS 2
#define BACKGROUND 3
#define TEXT_BACKGROUND 4
#define LINE_HIGHLIGHT 5
#define SIDE_STRIP_HIGHLIGHT 6
#define TITLE 7
#define LOWER_STRIP_BACKGROUND 8
#define LOWER_STRIP_TEXT 9

#ifndef CTRL
#define CTRL(c) (((int)c) & 037)
#endif

#define CTRL_LEFT 443
#define CTRL_RIGHT 444
#define CTRL_UP 480
#define CTRL_DOWN 481

#define SHIFT_LEFT 391
#define SHIFT_RIGHT 400
#define SHIFT_UP 547
#define SHIFT_DOWN 548

SIMPLE_GLOBAL bool Quit;
SIMPLE_GLOBAL simple_file *Current_File;
SIMPLE_GLOBAL unsigned int Txt_Disp_Start_X , Txt_Disp_Start_Y , Txt_Disp_End_X , Txt_Disp_End_Y;
SIMPLE_GLOBAL unsigned int Least_H_Distance , Least_V_Distance;
SIMPLE_GLOBAL unsigned int Screen_Width , Screen_Height;
SIMPLE_GLOBAL WINDOW *Text_Window , *Lower_Strip_Window;

#endif