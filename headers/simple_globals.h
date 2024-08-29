#ifndef SIMPLE_GLOBALS
#define SIMPLE_GLOBALS

#include "simple_file.h"

#include <stddef.h>

#define SIMPLE_GLOBAL __attribute__((__common__))

SIMPLE_GLOBAL simple_file *Current_File;
SIMPLE_GLOBAL size_t Txt_Start_Line;
SIMPLE_GLOBAL unsigned int Txt_Disp_Start_X , Txt_Disp_Start_Y , Txt_Disp_End_X , Txt_Disp_End_Y;
SIMPLE_GLOBAL size_t Txt_Disp_Indent;
SIMPLE_GLOBAL size_t Col_Pos , Line_Pos;//to know our position in the file

#endif