#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

void processFile(char *inputFilePath, char *outputFileName){
    char *currLine = NULL;
    size_t len = 0;
    int lineNum = 1;

    // open the input file for reading only
    FILE *inputFile = fopen(inputFilePath, "r");
    if (inputFile == NULL) {
        perror(inputFilePath);
        exit(EXIT_FAILURE);
    }

    // output file path starts with "outputDir/" which is 10 chars
    size_t numChars = 10 + strlen(outputFileName) + 1; // need + 1 for '\0'
    char *outputFilePath = malloc(numChars * sizeof(char));

    sprintf(outputFilePath, "%s/%s", "outputDir", outputFileName);

    FILE *outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL) {
        perror(outputFilePath);
        fclose(inputFile);
        free(outputFilePath);
        exit(EXIT_FAILURE);
    }

    // read the input file line by line and copy each line into the output file
    while(getline(&currLine, &len, inputFile) != -1){
        fprintf(outputFile, "%d,%s", lineNum, currLine);
        lineNum++;
    }
    free(currLine); // free memory allocated by getline

    free(outputFilePath);

    fclose(inputFile);
    fclose(outputFile);

}

void processCurrDir(void) {
    // open current directory
    DIR *currDir = opendir(".");
    struct dirent *entry;

    // loop through all entries in the current directory
    while ((entry = readdir(currDir)) != NULL) {
        printf("%s %lu\n", entry->d_name, entry->d_ino);
    }

    // close the directory
    closedir(currDir);
}

void usingStat(void) {
    // open current directory
    DIR *currDir  =opendir(".");
    struct dirent *entry;
    struct stat dirStat;

    // loop through all entries in the dir and print their name and time accessed
    while ((entry = readdir(currDir)) != NULL) {
        // get the metadata for the entry
        stat(entry->d_name, &dirStat);
        printf("%s modified at %ld\n", entry->d_name, dirStat.st_mtime);
    }

    // close the directory
    closedir(currDir);
}

int fileDescriptor(char *newFilePath) {
    int file_descriptor = 0;
    // flags: read and write, create an empty file if the file does not exist, truncate file if exists
    file_descriptor = open(newFilePath, O_RDWR | O_CREAT | O_TRUNC, 0600);

    if (file_descriptor == -1) {
        printf("open() failed on \"%s\"\n", newFilePath);
        perror(newFilePath);
        exit(1);
    }
    printf("File descriptor = %d\n", file_descriptor);

    return file_descriptor;
}

void statRegularFile(char *directory) {
    DIR *currDir = opendir(directory);
    struct dirent *entry;
    struct stat dirStat;

    while ((entry = readdir(currDir)) != NULL) {
        stat(entry->d_name, &dirStat);
        if (S_ISREG(dirStat.st_mode)) {
            printf("%s modified last at %ld\n", entry->d_name, dirStat.st_mtime);
        }
    }

    closedir(currDir);
}

int main(int argc, char **argv) {
    if (argc < 3){
        printf("You must provide names for input and output files. \n");
        return EXIT_FAILURE;
    } else{
        processFile(argv[1], argv[2]);
        //return EXIT_SUCCESS;
    }

    // current directory
    printf("--------------- Processing current dir ---------------\n");
    processCurrDir();
    // current directory stats
    printf("--------------- Stats on current dir ---------------\n");
    usingStat();
    // create a new file
    printf("--------------- Making a new file ---------------\n");
    int file_descriptor = fileDescriptor("./newFile.txt");
    close(file_descriptor);
    // modified time for regular file only
    printf("--------------- Time for a regular file ---------------\n");
    statRegularFile(".");


    return 0;

}