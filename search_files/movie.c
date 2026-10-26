//
// Created by lora on 10/17/26.
//
#include "movie.h"
#include <stdlib.h>
#include <string.h>

/* helper method: sopy the num_lang of languages from parsed languages to film->lang */
void addLangToNode(char film_lang[5][21], char** languages, int num_lang)
{
    for (int i = 0; i < num_lang; i++)
    {
        strcpy(film_lang[i], languages[i]);
    }
}

/* make a movie node, return pointer to movie node */
movie_t *createMovieNode(char* title, size_t title_len, int year, char** languages, int num_lang, float rating)
{
    movie_t *film = malloc(sizeof(movie_t));
    film->title = calloc(title_len + 1, sizeof(char));
    strcpy(film->title, title);
    film->title_len = title_len;
    film->year = year;
    film->num_lang = num_lang;
    addLangToNode(film->languages, languages, num_lang);
    film->rating = rating;
    film->next = NULL;

    return film;
}

/* free movie node, return null */
void freeMovieNode(movie_t *film)
{
    free(film->title);
    free(film);
}
