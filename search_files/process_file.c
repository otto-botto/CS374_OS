//
// Created by lora on 10/25/26.
//
#include "process_file.h"



/* concatenate onid, movies and random number and generate a directory
 * used rand() example in https://en.cppreference.com/c/numeric/random/rand
 */
void createDirectory(directory_t *directory) {
    srand(time(NULL));
    int random_num = rand()/MAX_RANDOM; // divide by max random to ensure number stays within range
    //printf("%d\n", random_num);
    char name[DIR_MAX_CHARS];
    sprintf(name,"%s.%s.%d", ONID, SUB, random_num);
    strcpy(directory->path_name, name);
    directory->path_length = strlen(name);
}

int processFile(collection_t *collection, set_t *yearsSet) {
    // create a directory
    directory_t directory = {0};
    createDirectory(&directory);
    int success_dir = mkdir(directory.path_name, 0750);
    printf("%s\n",directory.path_name);
    if (success_dir == -1) {
        printf("mkdir error\n");
        return -1;
    }

    // loop through set of years, make files in directory
    for (int i  = 0; i < yearsSet->num_years; i++) {
        // make the file
        char year[YEAR_FILE_CHARS]; // 4 digits + 4 for ".csv" + '\0'
        int current_year = yearsSet->years_set[i];
        sprintf(year, "%d", current_year);
        strcat(year, ".csv");
        //printf("%s and len: %lu\n",year, strlen(year));
        char filePath[DIR_MAX_CHARS + YEAR_FILE_CHARS + 1];
        sprintf(filePath, "%s/%s", directory.path_name, year);
        //printf("%s and length %lu\n",filePath, strlen(filePath));
        // make the file
        int file_descriptor;
        file_descriptor = open(filePath, O_RDWR | O_CREAT| O_TRUNC, 0640);
        if (file_descriptor == -1) {
            printf("open() failed on \"%s\"\n", filePath);
            perror(filePath);
            return -1;
        }else {
            movie_t *tmp = collection->head;
            while (tmp != NULL) {
                if (tmp->year == current_year) {
                    int bytes_written = write(file_descriptor, tmp->title, tmp->title_len);
                    write(file_descriptor, "\n", 1); // add new line between titles
                    if (bytes_written == -1) {
                        printf("write() failed on \"%s\"\n", tmp->title);
                        perror(tmp->title);
                        return -1;
                    }
                }
                tmp = tmp->next;
            }
            close(file_descriptor);
        }
    }
    return 0;
}