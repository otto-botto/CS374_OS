#include "gtest/gtest.h"

extern "C" {
#include "parser.h"
}

TEST(ParseFile, ParseFileTest)
{
    char string[] = "/home/lora/CLion/CS374/movies/test.csv";
    char **tokens = processMovieFile(string);

    EXPECT_STREQ("The Incredible Hulk", tokens[0]);
    EXPECT_STREQ("2008", tokens[1]);
    EXPECT_STREQ("[English;Portuguese;Spanish]", tokens[2]);
    EXPECT_STREQ("6.8\n", tokens[3]);

    freeTokens(tokens);
}

TEST(TokenizeLang, TokenizeLangTest)
{
    int num_lang = 0;
    char string[] = "[English;Portuguese;Spanish]";
    char **tokens = tokenizeLang(string, &num_lang);
    EXPECT_STREQ("English", tokens[0]);
    EXPECT_STREQ("Portuguese", tokens[1]);
    EXPECT_STREQ("Spanish", tokens[2]);
    EXPECT_EQ(3, num_lang);

    for (int i = 0; i < num_lang; i++)
    {
        printf("%s ",tokens[i]);
    }

    freeLangArray(tokens);

}

TEST(CreateMovie, CreateMovieTest)
{
    char string[] = "/home/lora/CLion/CS374/movies/test.csv";
    char **tokens = processMovieFile(string);
    movie_t *film = createMovie(tokens);

    EXPECT_STREQ("The Incredible Hulk", film->title);
    EXPECT_EQ(2008, film->year);
    EXPECT_EQ(3, film->num_lang);
    EXPECT_STREQ("English", film->languages[0]);
    EXPECT_STREQ("Portuguese", film->languages[1]);
    EXPECT_STREQ("Spanish", film->languages[2]);
    EXPECT_EQ(6.8f, film->rating);

}