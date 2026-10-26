//
// Created by lora on 10/17/26. I used the starter code in movies.c
//

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

/* parse the csv file a row at a time and return array of 4 string pointers */
char **processMovieFile(char *filePath)
{
    int line_num = 0;
    char *currLine = NULL;
    size_t line_len = 0;
    FILE *movieFile = fopen(filePath, "r");

    char **tokens = calloc(4, sizeof(char*));
    // read the file line by line
    while (getline(&currLine, &line_len, movieFile) != -1)
    {
        if (line_num == 0)
        {
            line_num++;
            continue;
        }
        char *savePtr;
        char *token = strtok_r(currLine, ",", &savePtr);
        int index = 0;

        while (token != NULL && index < 4)
        {
            size_t token_len = strlen(token);
            char *token_add = calloc(token_len + 1, sizeof(char));
            tokens[index] = token_add;
            strcpy(tokens[index], token);
            index++;
            token = strtok_r(NULL, ",", &savePtr);
        }
        line_num++;
    }


    // free memory allocated to currLine from getline()
    free(currLine);
    fclose(movieFile);
    return tokens;
}

/* helper fn: tokenize the string of languages & return array of up to 5 string, modify the num_lang int ptr */
char **tokenizeLang(char *languages, int *num_lang)
{
    // make the array of languages, up to 5
    char **lang_array = calloc(MAX_NUM_LANG, sizeof(char *));
    for (int i = 0; i < MAX_NUM_LANG; i++)
    {
        lang_array[i] = calloc(MAX_LANG_LEN + 1, sizeof(char));
    }
    char *savePtr;
    char * token = strtok_r(languages, ";", &savePtr);
    int index = 0;
    while (token != NULL)
    {
        if (index == 0)
        {
            strncpy(lang_array[index], token+1, MAX_LANG_LEN); // [English, skip '['
        }else
        {
            strncpy(lang_array[index], token,MAX_LANG_LEN);
        }
        index++;
        token = strtok_r(NULL, ";", &savePtr);
    }

    // cleanup the last ']'
    size_t last_str_len = strlen(lang_array[index - 1]);
    lang_array[index - 1][last_str_len - 1] = '\0';

    *num_lang = index;
    return lang_array;
}

/* free languages array */
void freeLangArray(char **lang_array)
{
    int index = 0;
    while (index < MAX_NUM_LANG)
    {
        free(lang_array[index]);
        index++;
    }
    free(lang_array);
}

/* free tokens array */
void freeTokens(char **tokens)
{
    int index = 0;
    while (index < 4)
    {
        free(tokens[index]);
        index++;
    }
    free(tokens);
}

/* make a movie_t Node from the strings in tokens */
movie_t *createMovie(char **tokens)
{
    char * title = tokens[0];
    size_t title_len = strlen(title);
    int year = atoi(tokens[1]);
    int num_lang = 0;
    char **languages = tokenizeLang(tokens[2], &num_lang);
    // clean up '\n' in rating
    size_t rate_len = strlen(tokens[3]);
    tokens[3][rate_len - 1] = '\0';
    float rating = atof(tokens[3]);
    movie_t *film = createMovieNode(title, title_len, year, languages, num_lang, rating);

    freeLangArray(languages);
    freeTokens(tokens);
    return film;
}

/* tokenize a line from csv file */
char **tokenizeLine(char *currLine)
{
    char **tokens = calloc(4, sizeof(char*));
    char *savePtr;
    char *token = strtok_r(currLine, ",", &savePtr);
    int index = 0;
    while (token != NULL && index < 4)
    {
        size_t token_len = strlen(token);
        char *token_add = calloc(token_len + 1, sizeof(char));
        strcpy(token_add, token);
        tokens[index] = token_add;
        index++;
        token = strtok_r(NULL, ",", &savePtr);
    }
    return tokens;
}