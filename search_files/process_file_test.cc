#include <gtest/gtest.h>

extern "C" {
    #include "process_file.h"
}


TEST(RandomNum, RandomNumTest) {
    directory_t dir = {0};
    createDirectory(&dir);
    printf("%s\n", dir.path_name);
    printf("%lu\n", dir.path_length);
}

TEST(ProcessFile, ProcessFileTest) {
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

    set_t *years = makeYearsSet(collection);
    int result = processFile(collection, years);
    EXPECT_EQ(result, 0);


    freeCollection(collection);
}
