#ifndef SIMPLE_GLOBALS
#define SIMPLE_GLOBALS

#include "simple_file.h"

#include <stddef.h>
#include <pdcurses.h>

#define SIMPLE_GLOBAL __attribute__((__common__))

#define DEFAULT_OUTLINE_COLOR 0xEC
#define DEFAULT_BACKGROUND_COLOR 0xEA
#define DEFAULT_TEXT_COLOR COLOR_WHITE

#define TEXT 1
#define SIDE_STRIPS 2
#define UPPER_STRIP 3

SIMPLE_GLOBAL simple_file *Current_File;
SIMPLE_GLOBAL unsigned int Txt_Disp_Start_X , Txt_Disp_Start_Y , Txt_Disp_End_X , Txt_Disp_End_Y;
SIMPLE_GLOBAL unsigned int Least_H_Distance , Least_V_Distance;
SIMPLE_GLOBAL size_t Col_Pos , Line_Pos;//to know our position in the file

#endif