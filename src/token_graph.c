#include "../c_datastructures/headers/dynamic_array.h"
#include "../headers/token_graph.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define IS_VALID(ch) (bool)(ch == 9 || (ch >= 20 && ch <= 255 && ch != 127))
#define FIND_ROOT(ch) ch == 9 ? 0 : (ch - 20) + (ch < 127)
#define ROOT_NO 236

typedef struct letter_node{
    char ch;
    unsigned int line , column;
    struct letter_node *next_in_line , *daughter_node;
}letter_node;

typedef struct line_st{
    unsigned int line_no;
    letter_node *first_letter;
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

    ret -> line_no = 1;
    
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

                ret -> line_no++;
            }

            continue;
        }

        root_index = FIND_ROOT(src[i]);
        new_node = alloc_letter_node(src[i] , line , column);
        if(tmp != NULL) tmp -> next_in_line = new_node;
        else{
            curr_line = (line_st **)calloc(1 , sizeof(line_st *));
            *curr_line = (line_st *)calloc(1 , sizeof(line_st));
            (*curr_line) -> line_no = line;
            (*curr_line) -> first_letter = new_node;

            dynamic_array_add_element(ret -> lines , curr_line);
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

void token_graph_add_letter(char ch , unsigned int line , unsigned int column , token_graph *graph_ptr){
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

        (*new_line) -> line_no = line;
        (*new_line) -> first_letter = new_node;
        dynamic_array_add_element(graph_ptr -> lines , new_line);

        graph_ptr -> line_no ++;
    }else{
        line_st **line_ptr = dynamic_array_get_element(graph_ptr -> lines , line);
        line_st *target_line = *line_ptr;
        free(line_ptr);

        
        for(letter_node *nd = target_line -> first_letter ; nd != NULL ; nd = nd -> next_in_line){
            if(nd -> next_in_line == NULL){
                if(column - nd -> column > 1){
                    free_letter_node(new_node);

                    return;
                }

                nd -> next_in_line = new_node;

                break;
            }
            
            if(nd -> next_in_line -> column >= column){
                new_node -> next_in_line = nd -> next_in_line;
                nd -> next_in_line = new_node;

                break;
            }
        }

        for(letter_node *nd = new_node -> next_in_line ; nd != NULL ; nd = nd -> next_in_line){
            nd -> column ++;
        }
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

void token_graph_add_newline(unsigned int line , unsigned int column , token_graph *graph_ptr){
    if(line >= graph_ptr -> line_no) return ;

    line_st **line_ptr = dynamic_array_get_element(graph_ptr -> lines , line);
    line_st *target_line = *line_ptr;
    free(line_ptr);

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

            break;
        }
    }

    if(target_node == NULL && line_len < column) return;

    line_st **new_line_ptr = (line_st **)calloc(1 , sizeof(line_st *));
    if(new_line_ptr == NULL) return ;

    *new_line_ptr = calloc(1 , sizeof(line_st));
    line_st *new_line = *new_line_ptr;
    free(new_line_ptr);

    new_line -> line_no = line + 1;
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

        (*line2) -> line_no = (*line1) -> line_no + 1;
        (*line2) -> first_letter = (*line1) -> first_letter;
        for(letter_node *nd = (*line2) -> first_letter ; nd != NULL ; nd = nd -> next_in_line){
            nd -> line++;
        }

        free(line1);
        free(line2);
    }

    dynamic_array_edit_element(graph_ptr -> lines , line + 1 , &new_line);
}