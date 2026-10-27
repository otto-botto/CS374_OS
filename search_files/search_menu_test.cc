#include <gtest/gtest.h>
#include <unistd.h>
#include <climits>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>


extern "C" {
#include "search_menu.h"
}

namespace fs = std::filesystem;

// to see where search_menu_test is running

TEST(LargestFile, PrintCwd) {
    char cwd[PATH_MAX];
    ASSERT_NE(getcwd(cwd, sizeof cwd), nullptr);
    std::cout <<"Test is running in: "<< cwd << std::endl;
}

TEST(MeetsCriteria, MeetsCriteriaTest) {
    char fileName1[] = "movies_america.csv";
    char fileName2[] = "films_europe.csv";
    char fileName3[] = "movies_europe.txt";
    char fileName4[] = "films.txt";
    char fileName5[] = "movies_.csv";
    char fileName6[] = "movies.csv";
    char fileName7[] = "";
    int result1 = meetsCriteria(fileName1, strlen(fileName1));
    int result2 = meetsCriteria(fileName2, strlen(fileName2));
    int result3 = meetsCriteria(fileName3, strlen(fileName3));
    int result4 = meetsCriteria(fileName4, strlen(fileName4));
    int result5 = meetsCriteria(fileName5, strlen(fileName5));
    int result6 = meetsCriteria(fileName6, strlen(fileName6));
    int result7 = meetsCriteria(fileName7, strlen(fileName7));
    EXPECT_EQ(0, result1);
    EXPECT_EQ(-1, result2);
    EXPECT_EQ(-1, result3);
    EXPECT_EQ(-1, result4);
    EXPECT_EQ(0, result5);
    EXPECT_EQ(-1, result6);
    EXPECT_EQ(-1, result7);
}

class LargestFileTest : public ::testing::Test {
protected:
    fs::path oldCwd;
    fs::path tmpDir;

    void SetUp() override {
        oldCwd = fs::current_path();
        tmpDir = fs::temp_directory_path() / "search_files_test";
        fs::remove_all(tmpDir);          // clear leftovers from a crashed run
        fs::create_directory(tmpDir);
        fs::current_path(tmpDir);        // same as chdir(); now "." is tmpDir

        makeFile("movies_small.csv", 10);
        makeFile("movies_medium.csv", 500);
        makeFile("movies_big.csv", 2000);
        makeFile("awesome_films.csv", 1500);
        makeFile("tricky.txt", 2500);
        makeFile("tricky.csv", 3500);
        makeFile("movies.csv", 3500);
        makeFile("alpha.csv", 2500);
    }

    void TearDown() override {
        fs::current_path(oldCwd);        // restore so other tests aren't affected
        fs::remove_all(tmpDir);
    }

    static void makeFile(const std::string& name, size_t bytes) {
        std::ofstream(name) << std::string(bytes, 'x');
    }
};

TEST_F(LargestFileTest, CountsFilesInDirectory) {
    file_t largest = {0};
    int processed_files = searchChoiceOne(tmpDir.c_str(), &largest);

    //printf("Dir name: %s\n", tmpDir.c_str());
    EXPECT_STREQ("movies_big.csv", largest.name);
    EXPECT_EQ(2000, largest.bytes);
    EXPECT_EQ(3, processed_files);
}

TEST_F(LargestFileTest, CountsFilesSmallest) {
    file_t smallest = {0};
    int processed_files = searchChoiceTwo(tmpDir.c_str(), &smallest);
    EXPECT_STREQ("movies_small.csv", smallest.name);
    EXPECT_EQ(10, smallest.bytes);
    EXPECT_EQ(3, processed_files);
}

TEST_F(LargestFileTest, ChecksEnteredFile) {
    file_t entered = {0};
    char file[]    = "awesome_films.csv";
    size_t file_len = strlen("awesome_films.csv");

    int processed_files = searchChoiceThree(file, file_len, tmpDir.c_str(), &entered);
    EXPECT_STREQ(entered.name, file);
    EXPECT_EQ(0, processed_files);
}






