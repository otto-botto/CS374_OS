//
// Created by lora on 10/24/26.
//

#ifndef SEARCH_FILES_SEARCH_MENU_H
#define SEARCH_FILES_SEARCH_MENU_H

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "collection.h"

#define MAX_FILE_LEN 255

typedef struct File {
    char name[MAX_FILE_LEN + 1];
    size_t nameLen;
    size_t bytes;
}file_t;

int meetsCriteria(char *fileName, size_t length);
int searchChoiceOne(const char *directory, file_t *selectedFile);
int searchChoiceTwo(const char *directory, file_t *selectedFile);
int searchChoiceThree(char *file_name, size_t name_len,const char *directory, file_t *selectedFile);

#endif //SEARCH_FILES_SEARCH_MENU_H