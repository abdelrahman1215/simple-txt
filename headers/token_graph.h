#ifndef TOKEN_GRAPH
#define TOKEN_GRAPH

typedef struct token_graph token_graph;

token_graph *new_token_graph(char *src);
void destroy_token_graph(token_graph *graph_ptr);

void token_graph_add_letter(token_graph *graph_ptr , char ch , unsigned int line , unsigned int column);
void token_graph_add_newline(token_graph *graph_ptr , unsigned int line , unsigned int column);
void token_graph_delete_letter(token_graph *graph_ptr , unsigned int line , unsigned int column);
void token_graph_delete_newline(token_graph *graph_ptr , unsigned int line);

#endif