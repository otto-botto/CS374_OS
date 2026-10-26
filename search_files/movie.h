//
// Created by lora on 10/17/26.
//

#ifndef MOVIES_MOVIE_H
#define MOVIES_MOVIE_H
#include <stddef.h>


struct movie;

typedef struct Movie
{
    char *title;
    size_t title_len;
    int year;
    char languages[5][21]; // +1 char for null terminator
    int num_lang;
    float rating;
    struct Movie *next;
}movie_t;

movie_t *createMovieNode(char* title, size_t title_len, int year, char** languages, int num_lang, float rating);
void addLangToNode(char film_lang[5][21], char** languages, int num_lang);
void freeMovieNode(movie_t *film);


#endif //MOVIES_MOVIE_H
