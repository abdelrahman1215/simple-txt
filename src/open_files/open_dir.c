#include "../../headers/curses_header.h"
#include "../../headers/simple_file.h"

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

    for(size_t i = 0 ; i < entry_no - 1 ; i++){
        for(size_t i = 0 ; i < entry_no - 1 ; i++){
            if(entries[i].is_dir == false && entries[i + 1].is_dir){
                dir_entry tmp = entries[i];
                entries[i] = entries[i + 1];
                entries[i + 1] = tmp;
            }
        }
    }

    free(path);
    closedir(directory);

    return entries;
}

void open_dir(char *dir_name){
    if(dir_name == NULL) return ;
    if(dir_name[0] == '\000') return ;

    size_t entry_no;
    dir_entry *entries = get_entries(dir_name , &entry_no);
    if(entry_no == 0) return ;

    free(entries);
}