#include "../c_datastructures/headers/dynamic_array.h"
#include "../headers/token_graph.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define IS_VALID(ch) (bool)(ch == 9 || (ch >= 20 && ch != 127))
#define FIND_ROOT(ch) ch == 9 ? 0 : (ch - 20) + (ch < 127)
#define ROOT_NO 236

typedef struct letter_node{
    char ch;
    unsigned int line , column;
    struct letter_node *next_in_line , *daughter_node;
}letter_node;

typedef struct line_st{
    letter_node *first_letter;
    letter_node *last_letter;
    letter_node *last_added_letter;
}line_st;

struct token_graph{
    unsigned int line_no;
    dynamic_array *lines;
    letter_node *roots[ROOT_NO];
};

letter_node *alloc_letter_node(char ch , unsigned int line , unsigned int column){
    letter_node *ret = (letter_node *)calloc(1 , sizeof(letter_node));
    if(ret == NULL) return NULL;

    ret -> ch = ch;
    ret -> line = line;
    ret -> column = column;

    return ret;
}

void free_line_ptr(line_st **line_ptr_ptr){
    if(line_ptr_ptr == NULL) return ;
    if(*line_ptr_ptr == NULL) return ;

    free(*line_ptr_ptr);
    free(line_ptr_ptr);
}

token_graph *new_token_graph(char *src){
    if(src == NULL) return NULL;

    token_graph *ret = (token_graph *)calloc(1 , sizeof(token_graph));
    if(ret == NULL) return NULL;

    ret -> lines = new_dynamic_array(sizeof(line_st *) , (free_func *)free_line_ptr);
    if(ret -> lines == NULL){
        free(ret);

        return NULL;
    }

    int root_index;
        
    line_st **curr_line;
    letter_node *new_node = NULL;
    letter_node *tmp = NULL;
    for(size_t i = 0 , column = 0 , line = 0; src[i] != '\000' ; i++){
        if(!IS_VALID((unsigned char)src[i])){
            if(src[i] == '\n'){
                column = 0;
                line ++;
                tmp = NULL;
            }

            continue;
        }

        root_index = FIND_ROOT(src[i]);
        new_node = alloc_letter_node(src[i] , line , column);
        if(tmp != NULL){
            tmp -> next_in_line = new_node;
            (*curr_line) -> last_letter = new_node;
            (*curr_line) -> last_added_letter = new_node;
        }else{
            curr_line = (line_st **)calloc(1 , sizeof(line_st *));
            *curr_line = (line_st *)calloc(1 , sizeof(line_st));
            (*curr_line) -> first_letter = new_node;
            (*curr_line) -> last_letter = new_node;
            (*curr_line) -> last_added_letter = new_node;

            dynamic_array_add_element(ret -> lines , curr_line);
            ret -> line_no++;
        }

        letter_node *root = ret -> roots[root_index];
        if(root == NULL){
            ret -> roots[root_index] = new_node;
        }else{
            for(letter_node *nd = root ; nd != NULL ; nd = nd -> daughter_node){
                if(nd -> daughter_node == NULL){
                    nd -> daughter_node = new_node;

                    break;
                }
            }
        }

        column ++;
        tmp = new_node;
    }

    return ret;
}

void free_letter_node(letter_node *node_ptr){
    if(node_ptr == NULL) return ;
    free(node_ptr);
}

void destroy_token_graph(token_graph *graph_ptr){
    if(graph_ptr == NULL) return ;

    destroy_dynamic_array(graph_ptr -> lines);
    for(int i = 0 ; i < ROOT_NO ; i++){
        for(letter_node *nd = graph_ptr -> roots[i] , *next_nd ; nd != NULL ; nd = next_nd){
            next_nd = nd -> daughter_node;
            free_letter_node(nd);
        }
    }

    free(graph_ptr);
}

void token_graph_add_letter(token_graph *graph_ptr , char ch , unsigned int line , unsigned int column){
    if(!IS_VALID(ch) || graph_ptr == NULL) return ;
    if(line > graph_ptr -> line_no || (line == graph_ptr -> line_no && column > 0)) return ;

    letter_node *new_node = alloc_letter_node(ch , line , column);
    if(new_node == NULL) return;

    if(line == graph_ptr -> line_no){
        if(column > 0){
            free_letter_node(new_node);

            return;
        }
        
        line_st **new_line = (line_st **)calloc(1 , sizeof(line_st *));
        *new_line = (line_st *)calloc(1 , sizeof(line_st));

        (*new_line) -> first_letter = new_node;
        (*new_line) -> last_letter = new_node;
        (*new_line) -> last_added_letter = new_node;
        dynamic_array_add_element(graph_ptr -> lines , new_line);

        graph_ptr -> line_no ++;
    }else{
        line_st **line_ptr = dynamic_array_get_element(graph_ptr -> lines , line);
        line_st *target_line = *line_ptr;
        size_t line_len = 0;
        if(target_line -> last_letter != NULL) line_len = target_line -> last_letter -> column + 1;
        free(line_ptr);

        if(column > line_len){
            free_letter_node(new_node);

            return;
        }else if (column == 0){
            new_node -> next_in_line = target_line -> first_letter;
            target_line -> first_letter = new_node;
        }else if(column == line_len){
            if(target_line -> last_letter != NULL){
                target_line -> last_letter -> next_in_line = new_node;
            }else target_line -> first_letter = new_node;
            target_line -> last_letter = new_node;
        }else if(column == target_line -> last_added_letter -> column + 1){
            new_node -> next_in_line = target_line -> last_added_letter -> next_in_line;
            target_line -> last_added_letter -> next_in_line = new_node;
        }else{
            for(letter_node *nd = target_line -> first_letter ; nd != NULL ; nd = nd -> next_in_line){
                if(nd -> next_in_line -> column >= column){
                    new_node -> next_in_line = nd -> next_in_line;
                    nd -> next_in_line = new_node;
    
                    break;
                }
            }
        }

        for(letter_node *nd = new_node -> next_in_line ; nd != NULL ; nd = nd -> next_in_line){
            nd -> column ++;
        }

        target_line -> last_added_letter = new_node;
    }

    int root_index = FIND_ROOT(ch);
    letter_node *root = graph_ptr -> roots[root_index];
    if(root == NULL || root -> line > line || (root -> line == line && root -> column > column)){
        graph_ptr -> roots[root_index] = new_node;
    }else{
        for(letter_node *nd = root ; ; nd = nd -> daughter_node){
            if(nd -> daughter_node == NULL){
                nd -> daughter_node = new_node;

                break;
            }
            
            if((nd -> daughter_node -> line == line && nd -> column >= column) || nd -> daughter_node -> line > line){
                new_node -> daughter_node = nd -> daughter_node;
                nd -> daughter_node = new_node;

                break;
            }
        }
    }
}

void token_graph_add_newline(token_graph *graph_ptr , unsigned int line , unsigned int column){
    if(graph_ptr == NULL) return ;
    if(line >= graph_ptr -> line_no) return ;

    line_st **line_ptr = dynamic_array_get_element(graph_ptr -> lines , line);
    line_st *target_line = *line_ptr;
    free(line_ptr);
    if(target_line -> last_letter != NULL){
        if(column > target_line -> last_letter -> column + 1) return ;
    }

    letter_node *target_node = NULL;
    size_t line_len = 0;
    if(column == 0){
        target_node = target_line -> first_letter;
        target_line -> first_letter = NULL;
    }else for(letter_node *nd = target_line -> first_letter ; nd != NULL ; nd = nd -> next_in_line , line_len ++){
        if(nd -> next_in_line == NULL){
            line_len ++;
            break;
        }

        if(nd -> next_in_line -> column == column){
            target_node = nd -> next_in_line;
            nd -> next_in_line = NULL;
            target_line -> last_letter = nd;
            if(target_line -> last_added_letter -> column >= target_node -> column) target_line -> last_added_letter = nd;

            break;
        }
    }

    line_st *new_line = calloc(1 , sizeof(line_st));

    new_line -> first_letter = target_node;

    for(letter_node *nd = new_line -> first_letter ; nd != NULL ; nd = nd -> next_in_line){
        nd -> line++;
        nd -> column -= column;
    }

    line_st *empty_line = (line_st *)calloc(1 , sizeof(line_st));
    if(empty_line == NULL){
        free(new_line);

        return;
    }

    dynamic_array_add_element(graph_ptr -> lines , &empty_line);

    //adding the new line in its place
    line_st **line1 = NULL , **line2 = NULL;
    for(long long i = dynamic_array_get_elements_no(graph_ptr -> lines) - 1 ; i - 1 > line ; i--){
        line1 = dynamic_array_get_element(graph_ptr -> lines , i - 1);
        line2 = dynamic_array_get_element(graph_ptr -> lines , i);

        (*line2) -> first_letter = (*line1) -> first_letter;
        for(letter_node *nd = (*line2) -> first_letter ; nd != NULL ; nd = nd -> next_in_line){
            nd -> line++;
        }

        free(line1);
        free(line2);
    }

    dynamic_array_edit_element(graph_ptr -> lines , line + 1 , &new_line);
    graph_ptr -> line_no++;
}

void token_graph_delete_letter(token_graph *graph_ptr , unsigned int line , unsigned int column){
    if(graph_ptr == NULL) return ;
    if(line >= graph_ptr -> line_no) return ;

    line_st **line_ptr = dynamic_array_get_element(graph_ptr -> lines , line);
    if(line_ptr == NULL) return ;

    line_st *target_line = *line_ptr;
    free(line_ptr);
    if(target_line -> first_letter == NULL) return ;
    if(column > target_line -> last_letter -> column) return ;

    letter_node *target_node = NULL;
    for(letter_node *nd = target_line -> first_letter ; nd -> next_in_line != NULL ; nd = nd -> next_in_line){
        if(nd -> next_in_line -> column == column){
            if(nd -> next_in_line == target_line -> last_letter) target_line -> last_letter = nd;
            if(nd -> next_in_line == target_line -> last_added_letter) target_line -> last_added_letter = nd;

            nd = nd -> next_in_line;
            target_node = nd;
            
            for(nd = nd -> next_in_line ; nd != NULL ; nd = nd -> next_in_line){
                nd -> column--;
            }

            break;
        }
    }

    if(target_node == NULL) return;

    int root_index = FIND_ROOT(target_node -> ch);
    for(letter_node **nd_ptr = &(graph_ptr -> roots[root_index]) ; *nd_ptr != NULL ; nd_ptr = &((*nd_ptr) -> daughter_node)){
        if((*nd_ptr) -> line == line && (*nd_ptr) -> column == column){
            *nd_ptr = (*nd_ptr) -> daughter_node;

            break;
        }
    }

    free(target_node);
}

//removes the newline at the end of a line
void token_graph_delete_newline(token_graph *graph_ptr , unsigned int line){
    if(graph_ptr == NULL) return ;
    if(line + 1 >= graph_ptr -> line_no) return ;

    line_st **line_ptr = dynamic_array_get_element(graph_ptr -> lines , line);
    letter_node *last_node = (*line_ptr) -> last_letter;
    size_t column_add = last_node -> column + 1;
    free(line_ptr);

    line_ptr = dynamic_array_get_element(graph_ptr -> lines , line + 1);
    last_node -> next_in_line = (*line_ptr) -> first_letter;
    for(letter_node *nd = (*line_ptr) -> first_letter ; nd != NULL ; nd = nd -> next_in_line){
        nd -> line--;
        nd -> column += column_add;
    }
    free(line_ptr);

    dynamic_array_remove_element(graph_ptr -> lines , line + 1);
    graph_ptr -> line_no--;

    line_ptr = dynamic_array_get_element(graph_ptr -> lines , line + 1);
    for(size_t i = line + 1 ; i < graph_ptr -> line_no ; i++ , line_ptr = dynamic_array_get_element(graph_ptr -> lines , i)){
        for(letter_node *nd = (*line_ptr) -> first_letter ; nd != NULL ; nd = nd -> next_in_line){
            nd -> line--;
        }
    }
}

linked_list *token_graph_search(token_graph *graph_ptr , const char *token){
    if(token == NULL) return NULL;
    if(token[0] == '\000') return NULL;
    size_t len = 0;
    for(; token[len] != '\000' ; len++){
        if(!IS_VALID(token[len])) return NULL;
    }

    linked_list *ret = new_linked_list();
    if(ret == NULL) return NULL;

    size_t index = FIND_ROOT(token[0]);
    letter_node *root = graph_ptr -> roots[index];
    tk_mention_coord *last_added = NULL;
    for(letter_node *nd = root ; nd != NULL ; nd = nd -> daughter_node){
        size_t i = 1;
        bool add_node = true;
        for(letter_node *node = nd -> next_in_line ; node != NULL && i < len ; node = node -> next_in_line , i++){
            if(node -> ch != token[i]){
                add_node = false;
                break;
            }
        }

        if(add_node){
            tk_mention_coord mention = {.line = nd -> line , .column = nd -> column};
            linked_list_add_node(&mention , sizeof(tk_mention_coord) , NULL , ret);
        }
    }

    return ret;
}