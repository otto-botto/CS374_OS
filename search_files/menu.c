//
// Created by lora on 10/19/26.
//
#include "menu.h"


/* print all titles for a given year */
void choiceOne(int year, collection_t *collection)
{
    movie_t *tmp = collection->head;
    int num_titles = 0;
    while (tmp != NULL)
    {
        if (year == tmp->year)
        {
            printf("%s\n", tmp->title);
            num_titles++;
        }
        tmp = tmp->next;
    }
    // got to the end, check if any were printed
    if (num_titles == 0)
    {
        printf("No data about movies released in the year %d.\n", year);
    }
}

/* print the max rating title for each year */
void choiceTwo(collection_t *collection)
{
    set_t *all_years = makeYearsSet(collection);
    // loop through years set
    for (int i = 0; i < all_years->num_years; i++)
    {
        int curr_year = all_years->years_set[i];
        movie_t *tmp = collection->head;
        movie_t *max = NULL;
        int matches = 0;
        while (tmp != NULL)
        {
            if (tmp->year == curr_year)
            {
                if (matches == 0)
                {
                    max = tmp;
                    matches++;
                }else
                {
                    if (tmp->rating > max->rating)
                    {
                        max = tmp;
                        matches++;
                    }else
                    {
                        matches++;
                    }
                }
            }
            tmp = tmp->next;
        }
        // 2008 7.9 Iron Man
        printf("%d %.2f %s\n", max->year, max->rating, max->title);
    }

    freeYearsSet(all_years);
}

/* print the movies that are in a given language */
void choiceThree(char *language, collection_t *collection)
{
    // loop through the collection
    movie_t *tmp = collection->head;
    int matched_lang = 0;
    while (tmp != NULL)
    {
        for (int i = 0; i < tmp->num_lang; i++)
        {
            if (strcmp(language, tmp->languages[i]) == 0)
            {
                matched_lang++;
                printf("%d %s\n", tmp->year, tmp->title);
            }
        }
        tmp = tmp->next;
    }

    if (matched_lang == 0)
    {
        printf("No data about movies released in %s\n", language);
    }
    // for a given film, loop through the languages
    // if language same as lang, print film year and film title


}
