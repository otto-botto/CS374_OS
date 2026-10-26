#include "gtest/gtest.h"

extern "C" {
#include "menu.h"
}

TEST(ChoiceOneTwoMovies, ChoiceOneTwoMoviesTest)
{
    char french[]    = "French";
    char english[]   = "English";
    char bulgarian[] = "Bulgarian";
    char *languages[] = {french, english, bulgarian};
    char title[] = "The Incredible Hulk\0";
    size_t len_title = strlen(title);
    movie_t *film1 = createMovieNode(title, len_title,2008, languages, 3, 6.8);

    char russian[]    = "Russian";
    char *language[] = {russian};
    char title2[] = "Anna Karenina\0";
    size_t len_title2 = strlen(title2);
    movie_t *film2 = createMovieNode(title2, len_title2,1995, language, 1, 7.9);

    collection_t *collection = createCollection();
    addToCollection(collection, film1);
    addToCollection(collection, film2);

    choiceOne(1995, collection);
    choiceOne(2004, collection);
    choiceOne(2008, collection);

    freeCollection(collection);

}
TEST(ChoiceThree, ChoiceThreeTest)
{
    char french[]    = "French";
    char english[]   = "English";
    char bulgarian[] = "Bulgarian";
    char *languages[] = {french, english, bulgarian};
    char title[] = "The Incredible Hulk\0";
    size_t len_title = strlen(title);
    movie_t *film1 = createMovieNode(title, len_title,2008, languages, 3, 6.8);

    char russian[]    = "Russian";
    char *languages2[] = {english, russian};
    char title2[] = "Anna Karenina\0";
    size_t len_title2 = strlen(title2);
    movie_t *film2 = createMovieNode(title2, len_title2,1995, languages2, 2, 7.9);

    char punjabi[] = "Punjabi";
    collection_t *collection = createCollection();
    addToCollection(collection, film1);
    addToCollection(collection, film2);
    choiceThree(english, collection);
    choiceThree(russian, collection);
    choiceThree(punjabi, collection);
    freeCollection(collection);
}