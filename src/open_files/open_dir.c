#include "../../headers/simple_globals.h"
#include "../../headers/curses_header.h"
#include "../../headers/simple_file.h"
#include "../../headers/simple_str.h"
#include "../../headers/open_files.h"
#include "../../headers/display.h"

#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct dir_entry{
    char entry_name[260];
    bool is_dir;
} dir_entry;

dir_entry *get_entries(const char *dir_name , size_t *get_entry_no){
    if(get_entry_no == NULL) return NULL;
    *get_entry_no = 0;

    DIR *directory = opendir(dir_name);
    if(directory == NULL) return NULL;

    struct dirent *entry;
    size_t entry_no = 0;

    for(entry = readdir(directory) ; entry != NULL ; entry = readdir(directory) , entry_no ++){}

    if(entry_no == 0) return NULL;
    *get_entry_no = entry_no;

    closedir(directory);
    directory = opendir(dir_name);

    bool add_slash = true;
    size_t dir_name_len = 0;
    for(size_t i = 0 ; dir_name[i] != '\000' ; i++ , dir_name_len++){
        if((dir_name[i] == '\\' || dir_name[i] == '/') && dir_name[i + 1] == '\000') add_slash = false;
    }

    size_t path_len = dir_name_len + add_slash + 260;
    char *path = calloc(path_len , sizeof(char));
    if(path == NULL){
        closedir(directory);

        return NULL;
    }

    strncpy(path , dir_name , path_len);
    if(add_slash){
        path[dir_name_len] = '/';
    }

    size_t add_start = strlen(path);

    dir_entry *entries = calloc(entry_no , sizeof(dir_entry));
    if(entries == NULL){
        free(path);
        closedir(directory);

        return NULL;
    }

    entry = readdir(directory);
    for(size_t i = 0 ; entry != NULL ; entry = readdir(directory) , i++){
        strncpy(entries[i].entry_name , entry -> d_name , 260);

        sprintf(path + add_start , "%s" , entry -> d_name);
        entries[i].is_dir = is_dir(path);
    }

    size_t index = 0;
    dir_entry *ret = calloc(entry_no , sizeof(dir_entry));
    if(ret == NULL){
        closedir(directory);
        free(entries);
        free(path);

        return NULL;
    }

    for(size_t i = 0 ; i < entry_no ; i++){
        if(entries[i].is_dir){
            ret[index] = entries[i];
            index++;
        }
    }

    for(size_t i = 0 ; i < entry_no ; i++){
        if(!(entries[i].is_dir)){
            ret[index] = entries[i];
            index++;
        }
    }

    //the two for loops above sort the entries by adding directory entries first then adding file entries

    closedir(directory);
    free(entries);
    free(path);

    return ret;
}

simple_file *make_disp_file(dir_entry *entries , size_t entry_no){
    loading_err error;
    simple_file *ret = load_from_str("" , &error);
    if(ret == NULL){
        return NULL;
    }

    size_t line = 0 , col = 0;
    for(size_t i = 1/*skip the ./ folder*/ ; i < entry_no ; i++){
        line = simple_file_get_curr_line(ret);
        col = simple_file_get_curr_column(ret);
        simple_file_add(ret , line , col , entries[i].entry_name);

        col = simple_file_get_curr_column(ret);
        if(entries[i].is_dir){
            simple_file_add(ret , line , col , "/");
            col++;
        }
        
        if(i + 1 < entry_no) simple_file_add(ret , line , col , "\n");
    }
    
    simple_file_move_to_xy(ret , 0 , 0);
    return ret;
}

void open_dir(char *dir_name){
    if(dir_name == NULL) return ;
    if(dir_name[0] == '\000') return ;

    size_t entry_no;
    dir_entry *entries = get_entries(dir_name , &entry_no);
    if(entries == NULL) return ;

    simple_file *disp_file = make_disp_file(entries , entry_no);
    if(disp_file == NULL){
        free(entries);

        return ;
    }

    text_display_info *info = new_text_disp_info();
    if(info == NULL){
        destroy_simple_file(disp_file);
        free(entries);

        return ;
    }

    render_background(stdscr , 0 , 0 , Screen_Width , Screen_Height , BACKGROUND);

    bool Break = false;
    simple_str *file_path = new_simple_str(dir_name);
    if(dir_name[strlen(dir_name) - 1] != '/' && dir_name[strlen(dir_name) - 1] != '\\'){
        simple_str_add(file_path , "/" , simple_str_get_strlen(file_path));
    }

    attron(COLOR_PAIR(SIDE_STRIPS));

    mvprintw(0 , 1 , "press escape to exit");    

    attroff(COLOR_PAIR(SIDE_STRIPS));

    for(int ch = getch() ; !Break ; ch = getch()){
        update_text_display(disp_file , info , stdscr , BACKGROUND , TEXT , LINE_HIGHLIGHT , SIDE_STRIPS , SIDE_STRIP_HIGHLIGHT , 3 , 0 , false , false , true , false , 1 , Screen_Width - 1 , 1 , Screen_Height);

        switch(ch){
            case KEY_UP:
                simple_file_move_nlines_up(disp_file , 1);

                break;

            case KEY_DOWN:
                simple_file_move_nlines_down(disp_file , 1);

                break;

            case '\e':
                destroy_simple_file(disp_file);
                destroy_simple_str(file_path);
                free(entries);
                free(info);

                if(Current_File == NULL) Quit = true;
                
                return ;

            case '\n':
            case '\r':
                size_t line = simple_file_get_curr_line(disp_file);
                dir_entry target = entries[line + 1/*because the ../ folder was skipped*/];
                simple_str_add(file_path , target.entry_name , simple_str_get_strlen(file_path));

                if(target.is_dir){
                    simple_str_add(file_path , "/" , simple_str_get_strlen(file_path));
                    char *tmp = simple_str_get_string(file_path);
                    
                    free(entries);
                    entries = get_entries(tmp, &entry_no);
                    if(entries == NULL){
                        destroy_simple_file(disp_file);
                        destroy_simple_str(file_path);
                        free(info);

                        return ;
                    }

                    destroy_simple_file(disp_file);
                    disp_file = make_disp_file(entries , entry_no);
                    if(disp_file == NULL){
                        destroy_simple_str(file_path);
                        free(entries);
                        free(info);

                        return ;
                    }

                    break;
                }

                char *tmp_path = simple_str_get_string(file_path);
                open_file(tmp_path);
                Break = true;

                free(tmp_path);

                break;
        }
    }

    destroy_simple_file(disp_file);
    destroy_simple_str(file_path);
    free(entries);
    free(info);
}