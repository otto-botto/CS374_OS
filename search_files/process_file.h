//
// Created by lora on 10/25/26.
//

#ifndef SEARCH_FILES_PROCESS_FILE_H
#define SEARCH_FILES_PROCESS_FILE_H
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "collection.h"


#define MIN_RANDOM 0
#define MAX_RANDOM 99999
#define ONID "dushanol"
#define SUB "movies"
#define DIR_MAX_CHARS 255
#define YEAR_FILE_CHARS 9

typedef struct Directory {
    char path_name[DIR_MAX_CHARS];
    size_t path_length;
}directory_t;



int processFile(collection_t *collection, set_t *yearsSet);
void createDirectory(directory_t *directory);


#endif //SEARCH_FILES_PROCESS_FILE_H