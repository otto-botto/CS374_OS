#include "gtest/gtest.h"

extern "C" {
#include "collection.h"
}


TEST(CreateCollection, CreateCollectionTest)
{
    collection_t *collection = createCollection();
    EXPECT_EQ(nullptr, collection->head);
    EXPECT_EQ(nullptr, collection->tail);
    EXPECT_EQ(0, collection->size);
    freeCollection(collection);
}

TEST(AddOneToCollection, AddOneToCollectionTest)
{
    // manually make a film
    char french[]    = "French";
    char english[]   = "English";
    char bulgarian[] = "Bulgarian";
    char *languages[] = {french, english, bulgarian};
    char title[] = "The Incredible Hulk\0";
    size_t len_title = strlen(title);
    movie_t *film = createMovieNode(title, len_title,2008, languages, 3, 6.8);
    collection_t *collection = createCollection();
    addToCollection(collection, film);


    EXPECT_EQ(film, collection->head);
    EXPECT_EQ(film, collection->tail);
    EXPECT_EQ(1, collection->size);
    EXPECT_STREQ(film->title, collection->head->title);
    EXPECT_STREQ(film->title, collection->tail->title);

    freeCollection(collection);
}

TEST(AddManyToCollection, AddManyToCollectionTest)
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

    EXPECT_EQ(film2, collection->head);
    EXPECT_EQ(film1, collection->tail);
    EXPECT_EQ(2, collection->size);
    EXPECT_STREQ(film2->title, collection->head->title);
    EXPECT_STREQ(film1->title, collection->tail->title);

    freeCollection(collection);

}

TEST(PrintCollection, PrintCollectionTest)
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

    printCollection(collection);
    freeCollection(collection);
}

TEST(SetYears, SetYearsTest)
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


    char greek[]    = "greek";
    char *language2[] = {greek};
    char title3[] = "War and Peace\0";
    size_t len_title3 = strlen(title3);
    movie_t *film3 = createMovieNode(title3, len_title3,1995, language2, 1, 3.1);


    collection_t *collection = createCollection();
    addToCollection(collection, film1);
    addToCollection(collection, film2);
    addToCollection(collection, film3);
    set_t *years = makeYearsSet(collection);
    EXPECT_EQ(2, years->num_years);
    for (int i = 0; i < years->num_years; i++)
    {
        printf("[%d] %d ", i, years->years_set[i]);
    }
    EXPECT_EQ(1995, years->years_set[0]);
    EXPECT_EQ(2008, years->years_set[1]);

    freeYearsSet(years);
    freeCollection(collection);
}
