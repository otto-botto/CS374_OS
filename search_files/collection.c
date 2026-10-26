//
// Created by lora on 10/18/26.
//
#include "collection.h"

/* create collection, returns a head with only next pointer */
collection_t *createCollection(void)
{
    collection_t *collection = malloc(sizeof(collection_t));
    collection->head = NULL;
    collection->tail = NULL;
    collection->size = 0;
    return collection;
}

void addToCollection(collection_t *collection, movie_t *film)
{
    film->next = collection->head; // NULL if first film
    collection->head = film;
    // set tail for the first push
    if (collection->size == 0)
    {
        collection->tail = collection->head;
    }
    collection->size++;
}

void freeCollection(collection_t *collection)
{
    movie_t *first_film = collection->head;
    while (first_film)
    {
        movie_t *next_film = first_film->next;
        freeMovieNode(first_film);
        first_film = next_film;
    }
    free(collection);
}

void printCollection(collection_t *collection)
{
    movie_t *tmp = collection->head; // don;t move the actual head, use tmp to print ALWAYS
    while (tmp != NULL)
    {

        printf("%s, %d, %0.2f, languages: ", tmp->title, tmp->year, tmp->rating);
        int num_lang = tmp->num_lang;
        for (int j = 0; j < num_lang; j++)
        {
            if (j == (num_lang - 1)) {printf("%s.", tmp->languages[j]);}
            else {printf("%s, ", tmp->languages[j]);}
        }
        putchar('\n');
        tmp = tmp->next;
    }
}

collection_t *loadCollection(char *file_name)
{
    FILE *movieFile = fopen(file_name, "r");
    if (movieFile == NULL)
    {
        perror(file_name);
        return NULL;
    }

    collection_t *collection = createCollection();
    int line_num = 0;
    char *currLine = NULL;
    size_t line_len = 0;

    // read the file line by line
    while (getline(&currLine, &line_len, movieFile) != -1)
    {
        // skip headings
        if (line_num == 0){
            line_num++;
            continue;
        }

        char **tokens = tokenizeLine(currLine);

        movie_t *film = createMovie(tokens);
        addToCollection(collection, film);
        line_num++;
    }
    // free memory allocated to currLine from getline()
    free(currLine);
    fclose(movieFile);
    return collection;
}

set_t *makeYearsSet(collection_t *collection)
{
    int size = collection->size;
    set_t *yrs = malloc(sizeof(set_t));
    yrs->years_set = calloc(size, sizeof(int));
    yrs->num_years = 0;

    movie_t *tmp = collection->head;
    while (tmp != NULL)
    {
        int index = 0;
        if (yrs->num_years == 0)
        {
            yrs->years_set[index] = tmp->year;
            yrs->num_years++;
        }else
        {
            while (index < yrs->num_years)
            {
                if (yrs->years_set[index] != tmp->year)
                {
                    index++;
                }else
                {
                    break;
                }
            }
            if (index == yrs->num_years)
            {
                yrs->years_set[index] = tmp->year;
                yrs->num_years++;
            }
        }
        tmp = tmp->next;
    }
    return yrs;
}

void freeYearsSet(set_t *years)
{
    free(years->years_set);
    free(years);
}