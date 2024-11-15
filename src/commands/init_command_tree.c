#include "command_tree.c"
#include "commands.c"

#include <string.h>

command_info command_arr[] = {
    {.command = "w" , .command_exec = write_to_current_file , .token_no = 0} ,
    {.command = "q" , .command_exec = quit , .token_no = 0} ,
    {.command = "wq" , .command_exec = write_and_quit , .token_no = 0} ,
    {.command = "edit" , .command_exec = edit_file , .token_no = 1} ,
    {.command = "relative" , .command_exec = relative , .token_no = 0},
    {.command = "ln", .command_exec = move_to_line , .token_no = 1},
    {.command = "col", .command_exec = move_to_column , .token_no = 1},
    {.command = "man", .command_exec = manual , .token_no = 0}
};

command_tree *Command_Tree = NULL;

void init_command_tree(){
    if(Command_Tree != NULL) return;

    Command_Tree = new_command_tree();
    if(Command_Tree == NULL) return;

    int command_no = sizeof(command_arr)/sizeof(command_info);
    command_info target;
    for(int i = 0 ; i < command_no ; i++){
        target = command_arr[i];
        command_tree_add_node(Command_Tree , target.command , target.token_no , target.command_exec);
    }
}