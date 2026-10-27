//
// Created by lora on 10/24/26.
//

#include "search_menu.h"

int meetsCriteria(char *fileName, size_t length) {
    // moves_ is 6 chars and .csv is 4 ==> fewer than 10 chars
    if (length <11) {
        return -1;
    }else {
        //check ending
        char ending[5]; // ".csv" + \0
        int index = 0;
        for (size_t i = length - 4; i < length; i++) {
            ending[index] = fileName[i];
            index++;
        }
        ending[4] = '\0';
        // check beginning
        char beginning[8]; // "movies_" + '\0'
        strncpy(beginning, fileName, 7);
        beginning[7] = '\0';
        //printf("ending: %s\n", ending);
        //printf("beginning: %s\n", beginning);
        if ((strcmp(ending,".csv") == 0) &&(strcmp(beginning, "movies_") == 0)) {
            return 0;
        }
    }
    return -1;
}

int searchChoiceOne(const char *directory, file_t *selectedFile) {
    DIR *currDir = opendir(directory);
    if (currDir == NULL) {
        perror(directory);
        return -1;
    }
    struct dirent *entry;
    struct stat tmpStat; // for looping through the directory
    int numFilesProcessed = 0;

    // loop through entry(ies) in dir
    while ((entry = readdir(currDir)) != NULL) {

        // if could not read stat, not csv file, not regular file, move onto next file
        if (stat(entry->d_name, &tmpStat) != 0) {
            continue;
        } else if (meetsCriteria(entry->d_name, strlen(entry->d_name)) != 0) {
            continue;
        } else if (!S_ISREG(tmpStat.st_mode)) {
            continue;
        }else {
            if ( numFilesProcessed == 0) {
                strcpy(selectedFile->name, entry->d_name);
                selectedFile->nameLen = strlen(entry->d_name);
                selectedFile->bytes = tmpStat.st_size;
            }else {
                if (tmpStat.st_size > selectedFile->bytes) {
                    strcpy(selectedFile->name, entry->d_name);
                    selectedFile->nameLen = strlen(entry->d_name);
                    selectedFile->bytes = tmpStat.st_size;
                }
            }
            numFilesProcessed++;
        }
    }
    // close the directory
    closedir(currDir);
    return numFilesProcessed;
}

int searchChoiceTwo(const char *directory, file_t *selectedFile)
{
    DIR *currDir = opendir(directory);
    if (currDir == NULL)
    {
        perror(directory);
        return -1;
    }
    struct dirent *entry;
    struct stat tmpStat;
    int numFilesProcessed = 0;

    while ((entry = readdir(currDir)) != NULL)
    {

        // if could not read, doesn't meet criteria, or not regular --> continue
        if (stat(entry->d_name, &tmpStat) != 0)
        {
            continue;
        }
        else if (meetsCriteria(entry->d_name, strlen(entry->d_name)) != 0)
        {
            continue;
        }
        else if (!S_ISREG(tmpStat.st_mode))
        {
            continue;
        }
        else
        {
            if (numFilesProcessed == 0) {
                strcpy(selectedFile->name, entry->d_name);
                selectedFile->nameLen = strlen(entry->d_name);
                selectedFile->bytes = tmpStat.st_size;
            }else {
                if (tmpStat.st_size < selectedFile->bytes) {
                    strcpy(selectedFile->name, entry->d_name);
                    selectedFile->nameLen = strlen(entry->d_name);
                    selectedFile->bytes = tmpStat.st_size;
                }
            }
            numFilesProcessed++;
        }
    }

    closedir(currDir);
    return numFilesProcessed;
}

int searchChoiceThree(char *file_name, size_t name_len,const char *directory, file_t *selectedFile) {
    DIR *currDir = opendir(directory);
    if (currDir == NULL) {
        perror(directory);
        return -1;
    }

    struct dirent *entry;
    struct stat tmpStat;

    while ((entry = readdir(currDir)) != NULL) {
        if (stat(entry->d_name, &tmpStat) != 0) {
            continue; // could not read stats
        }
        else if (!S_ISREG(tmpStat.st_mode)) {
            continue; // not a regular file
        }
        else if (strcmp(file_name, entry->d_name) != 0) {
            continue; // not the entered file
        }
        else {
            // set up the selected file
            strcpy(selectedFile->name, file_name);
            selectedFile->nameLen = name_len;
            selectedFile->bytes = tmpStat.st_size;
            return 0;
        }
    }


    closedir(currDir);
    return -1; // did not find the file
}
