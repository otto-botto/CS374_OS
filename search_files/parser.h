//
// Created by lora on 10/17/26.
//

#ifndef MOVIES_PARSER_H
#define MOVIES_PARSER_H
#include "movie.h"

#define MAX_LANG_LEN 20
#define MAX_NUM_LANG 5

char **processMovieFile(char *filePath);
void freeTokens(char **tokens);
char **tokenizeLang(char *languages, int *num_lang);
void freeLangArray(char **lang_array);
movie_t *createMovie(char **tokens);
char **tokenizeLine(char *currLine);

#endif //MOVIES_PARSER_H
