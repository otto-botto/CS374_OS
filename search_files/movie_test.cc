#include "gtest/gtest.h"

extern "C" {
#include "movie.h"
}

TEST(AddLang, AddLangTest) {
    char french[]    = "French";
    char english[]   = "English";
    char bulgarian[] = "Bulgarian";
    char *languages[] = {french, english, bulgarian};   // decays to char**

    char film_lang[5][21] = {};   // zero-initialized: every row is ""

    addLangToNode(film_lang, languages, 3);
    EXPECT_STREQ("French", film_lang[0]);
    EXPECT_STREQ("English", film_lang[1]);
}

TEST(MakeMovie, MakeMovieTest) {
    char title[] = "The Incredible Hulk\0";
    size_t title_len = strlen(title);
    int year = 2008;
    char french[] = "French";
    char english[] = "English";
    char bulgarian[] = "Bulgarian";
    char *languages[] = {french, english, bulgarian};   // decays to char**
    int num_lang = 3;
    float rating = 6.8;
    movie_t *film = createMovieNode(title, title_len,year,
        languages, num_lang, rating);

    EXPECT_STREQ("The Incredible Hulk", film->title);
    EXPECT_EQ(19, film->title_len);
    EXPECT_EQ(2008, film->year);
    EXPECT_STREQ("French", film->languages[0]);
    EXPECT_STREQ("English", film->languages[1]);
    EXPECT_STREQ("Bulgarian", film->languages[2]);
    EXPECT_EQ(3, film->num_lang);
    EXPECT_EQ(6.8f, film->rating);
    EXPECT_EQ(nullptr, film->next);
    free(film->title);
    free(film);
}

TEST(FreeMovie, FreeMovieTest)
{
    char title[] = "The Incredible Hulk\0";
    size_t title_len = strlen(title);
    int year = 2008;
    char french[] = "French";
    char english[] = "English";
    char bulgarian[] = "Bulgarian";
    char *languages[] = {french, english, bulgarian};   // decays to char**
    int num_lang = 3;
    float rating = 6.8;
    movie_t *film = createMovieNode(title, title_len,year,
        languages, num_lang, rating);
    freeMovieNode(film);
}