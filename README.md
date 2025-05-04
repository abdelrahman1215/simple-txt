# simple-txt
a vim inspired text editor written from scratch in c using the curses library (pdcurses for windows and ncurses for linux)

* [building](https://github.com/abdelrahman1215/simple-txt?tab=readme-ov-file#building)

* [screenshots/records](https://github.com/abdelrahman1215/simple-txt?tab=readme-ov-file#screenshots--screen-records)

* [controls](https://github.com/abdelrahman1215/simple-txt?tab=readme-ov-file#controls)

* [commands](https://github.com/abdelrahman1215/simple-txt?tab=readme-ov-file#commands)

* [flags/options](https://github.com/abdelrahman1215/simple-txt?tab=readme-ov-file#flagsoptions)

# building
## prequisites
* for windows : 
    * [pdcurses](https://sourceforge.net/projects/pdcurses/)
    * install libmagic
    > if you are using msys2 install mingw-w64-x86_64-file (mingw-w64-ucrt-x86_64-file if you are using UCRT)

* for linux :
    * ncurses and libmagic packages

## cloning
```
git clone https://github.com/abdelrahman1215/simple-txt --recurse-submodules
```

## compiling
* for windows
```
make windows
```

* for linux
```
make linux
```

# screenshots / screen records
![screenshot1](https://github.com/abdelrahman1215/simple-txt/blob/main/demos/Screenshot%20(7).png?raw=true)

>note: the actual cursor is hidden , this cursor is displayed by the program

![record1](https://github.com/abdelrahman1215/simple-txt/blob/main/demos/rec.gif?raw=true)

# controls
## navigation
vim like navigation controls
## editing
* ctrl z / ctrl y -> undo redo
* ctrl d -> delete a whole line

## command autocomplete
* TAB

# commands
* edit -> open files or directories
* w -> save changes
* q -> quit
* wq or x -> save & quit
* relative -> turn relative line numbers on/off
* ln -> jump to the nth line
* col -> jump to the nth column
* man -> get the manual page
* pwd -> get the current working directory
* cd -> change the current directory directory

# flags/options
* -v/--version -> program version
* -h/--help -> get manual page
