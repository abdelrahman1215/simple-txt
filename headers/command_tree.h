#ifndef COMMAND_TREE
#define COMMAND_TREE

typedef void(command_func) (char **tokens);

typedef struct command_info{
    char *command;
    size_t command_len;
    //specifies the number of tokens that should follow a command and be given to it
    unsigned short token_no;

    command_func *command_exec;
}command_info;

typedef struct command_tree command_tree;

command_tree *new_command_tree();
void destroy_command_tree(command_tree *tree_ptr);

void command_tree_add_node(command_tree *tree_ptr , const char *command , unsigned short token_no , command_func *command_exec);
void command_tree_delete_node(command_tree *tree_ptr , const char *command);

command_info command_tree_find_command(command_tree *tree_ptr , const char *command);

#endif