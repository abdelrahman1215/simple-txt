#ifndef MANUAL

#define MANUAL "\
Usage : ./(simple-txt executable) [options/flags] file\n\
\n\
Flags :\n\
    -h / --help : get the help page\n\
    -v / --version : get the program's version\n\
\n\
Options :\n\
\n\
Navigation :\n\
    arrow keys\n\
    h,j,k & l : move left, up, down & right respectively (in normal mode only)\n\
\n\
Bindings :\n\
    i (lower case) : enter insert mode\n\
    : (shift + ;) : enter command mode\n\
    (number) + h,j,k or l [eg : 12j] : move (number) left, up, down or right respectively (works in normal mode only)\n\
    TAB : autocomplete in command mode\n\
\n\
Editing :\n\
    CTRL-v : paste\n\
    CTRL-z / CTRL-y : undo / redo\n\
    CTRL-d : delete the whole line\n\
\n\
Commands :\n\
    w : save changes\n\
    q : quit program\n\
    wq : save and quit\n\
    edit (file name) : open a file / directory\n\
    relative : enable / disable relative numbers\n\
    ln (line number) : jump to line (number)\n\
    col (column number) : jump to column (number)\n\
    man : get the manual page\n\
\
"

char *get_manual();

#endif