#ifndef TOKEN_GRAPH
#define TOKEN_GRAPH

#include "../c_datastructures/headers/linked_list.h"

typedef struct token_graph token_graph;
typedef struct tk_mention_coord{
    unsigned int line , column;
} tk_mention_coord ;

token_graph *new_token_graph(char *src);
void destroy_token_graph(token_graph *graph_ptr);

void token_graph_add_letter(token_graph *graph_ptr , char ch , unsigned int line , unsigned int column);
void token_graph_add_newline(token_graph *graph_ptr , unsigned int line , unsigned int column);
void token_graph_delete_letter(token_graph *graph_ptr , unsigned int line , unsigned int column);
void token_graph_delete_newline(token_graph *graph_ptr , unsigned int line);

linked_list *token_graph_search(token_graph *graph_ptr , const char *token);

void print_graph(token_graph *graph_ptr);

#endif
