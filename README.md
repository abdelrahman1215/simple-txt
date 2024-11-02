# simple-txt
a vim inspired text editor written from scratch in c using pdcurses

# building
## prequisites
* for windows : 
    * [pdcurses](https://sourceforge.net/projects/pdcurses/)
* for linux 
    * ncurses package

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
![screenshot1](https://github.com/abdelrahman1215/simple-txt/blob/main/demos/Screenshot%20(7).png)

>note: the actual cursor is hidden , this cursor is displayed by the program

https://github.com/user-attachments/assets/56f12c0b-a72f-49d2-826e-42f5ee42f099

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
* wq -> save & quit
* relative -> turn relative line numbers on/off

# flags/options
* -v/--version -> program version
