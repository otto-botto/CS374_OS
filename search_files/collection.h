//
// Created by lora on 10/18/26.
//

#ifndef MOVIES_COLLECTION_H
#define MOVIES_COLLECTION_H
#include <stdlib.h>
#include <stdio.h>
#include "movie.h"
#include "parser.h"

/* To make a linked list with a head and a tail, I referenced Dr. James Aspnes Data Structures in C
 * https://www.cs.yale.edu/homes/aspnes/classes/223/notes.html, specifically his section 5.4.2 Queues.
 * I have been studying his material for some time, trying to learn data structures in C.
 */

typedef struct Collection {
    movie_t* head;
    movie_t* tail;
    int size;
}collection_t;

typedef struct Set
{
    int *years_set;
    int num_years;
}set_t;

collection_t *createCollection(void);
void addToCollection(collection_t *collection, movie_t *film);
void freeCollection(collection_t *collection);
void printCollection(collection_t *collection);
collection_t *loadCollection(char *file_name);
set_t *makeYearsSet(collection_t *collection);
void freeYearsSet(set_t *years);

#endif //MOVIES_COLLECTION_H
