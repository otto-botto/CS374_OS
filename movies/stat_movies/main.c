#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

typedef struct Movies {
    int num;
    char *data;
}movies_t;


movies_t *processMovies(int bytes, char *filePath) {
    int num_movies = 0;
    size_t len = 0;
    char *currLine = NULL;
    FILE *movieFile = fopen(filePath, "r");

    movies_t *films = calloc(1, sizeof(movies_t));
    films->data = calloc(bytes + 1, sizeof(char));
    films->num = 0;
    int line = 0;

    while (getline(&currLine, &len, movieFile) != -1 ) {
        if (line == 0) {
            line++; // skip the headings row
        }else {
            int index_carriage = strcspn(currLine, "\r\n");
            currLine[index_carriage] = '\0'; // remove \r and \n
            strcat(films->data, currLine); // concat the line
            strcat(films->data, "\n"); // add back and new line
            films->num++;
        }


    }

    free(currLine);
    fclose(movieFile);

    return films;
}


int main(int argc, char *argv[]) {
    struct stat sb;

    if (stat("/home/lora/CS374_OS/movies/stat_movies/movies_sample_1.csv", &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }else {
        printf("File size: %jd in bytes.\n", (intmax_t)sb.st_size);
    }

    long bytes = sb.st_size;
    movies_t *films = processMovies(bytes, "/home/lora/CS374_OS/movies/stat_movies/movies_sample_1.csv");

    printf("Num films %d\n", films->num);
    printf("%s\n", films->data);

    free(films->data);
    free(films);

    return 0;
}