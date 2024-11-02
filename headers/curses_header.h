#ifndef CURSES_HEADER
#define CURSES_HEADER

#ifdef LINUX
#include <ncursesw/curses.h>
#include <ncursesw/panel.h>
#else
#include <pdcurses.h>
#endif

#endif