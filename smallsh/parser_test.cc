#include <gtest/gtest.h>
#include <cstring>

extern "C"{
    #include "parser.h"
}

TEST(Hello, HelloTest) {
    int result = hello();
    EXPECT_EQ(3, result);
}

class ParserTest : public ::testing::Test {
protected:
    char buf[INPUT_LENGTH];
    command_line_t *cmd = nullptr;

    // strtok writes into the string, so copy into a writable buffer
    void parse(const char *input) {
        strncpy(buf, input, sizeof(buf) - 1);
        buf[sizeof(buf) - 1] = '\0';
        cmd = parse_line(buf);
        ASSERT_NE(cmd, nullptr);
    }

    void TearDown() override { free_command_line(cmd); }
};

TEST_F(ParserTest, SimpleCommand) {
    parse("ls\n");                       // fgets keeps the newline
    EXPECT_EQ(cmd->argc, 1);
    EXPECT_STREQ(cmd->argv[0], "ls");
    EXPECT_EQ(cmd->argv[1], nullptr);
    EXPECT_EQ(cmd->input_file, nullptr);
    EXPECT_EQ(cmd->output_file, nullptr);
    EXPECT_EQ(cmd->argv[2], nullptr);
    EXPECT_FALSE(cmd->is_bg);
}

TEST_F(ParserTest, CommandWithArgs) {
    parse("ls -l -a /tmp\n");
    ASSERT_EQ(cmd->argc, 4);
    EXPECT_STREQ(cmd->argv[0], "ls");
    EXPECT_STREQ(cmd->argv[1], "-l");
    EXPECT_STREQ(cmd->argv[2], "-a");
    EXPECT_STREQ(cmd->argv[3], "/tmp");
    EXPECT_EQ(cmd->argv[4], nullptr);
    EXPECT_FALSE(cmd->is_bg);
}

TEST_F(ParserTest, InputRedirection) {
    parse("wc -l < in.txt\n");
    EXPECT_EQ(cmd->argc, 2);
    EXPECT_STREQ(cmd->input_file, "in.txt");
    EXPECT_EQ(cmd->output_file, nullptr);
    EXPECT_EQ(cmd->argv[4], nullptr);
    EXPECT_FALSE(cmd->is_bg);
}

TEST_F(ParserTest, OutputRedirection) {
    parse("ls > out.txt\n");
    EXPECT_EQ(cmd->argc, 1);
    EXPECT_STREQ(cmd->output_file, "out.txt");
    EXPECT_EQ(cmd->argv[3], nullptr);
    EXPECT_FALSE(cmd->is_bg);
}

TEST_F(ParserTest, BothRedirectionsAndBackground) {
    parse("sort < in.txt > out.txt &\n");
    EXPECT_EQ(cmd->argc, 1);
    EXPECT_STREQ(cmd->argv[0], "sort");
    EXPECT_STREQ(cmd->input_file, "in.txt");
    EXPECT_STREQ(cmd->output_file, "out.txt");
    EXPECT_TRUE(cmd->is_bg);
    EXPECT_EQ(cmd->argv[6], nullptr);
}

TEST_F(ParserTest, RedirectionFilesNotInArgv) {
    parse("cat < a.txt\n");
    EXPECT_EQ(cmd->argc, 1);
    EXPECT_STREQ(cmd->input_file, "a.txt");
    EXPECT_EQ(cmd->argv[1], nullptr);
}

TEST_F(ParserTest, ExtraWhitespace) {
    parse("   echo    hi   \n");
    EXPECT_EQ(cmd->argc, 2);
    EXPECT_STREQ(cmd->argv[1], "hi");
}

TEST_F(ParserTest, EmptyLine) {
    parse("\n");
    EXPECT_EQ(cmd->argc, 0);
    EXPECT_EQ(cmd->argv[0], nullptr);
}

// Regression tests for the bugs fixed above
TEST_F(ParserTest, DanglingInputRedirectDoesNotCrash) {
    parse("cat <\n");
    EXPECT_EQ(cmd->argc, 1);
    EXPECT_EQ(cmd->input_file, nullptr);
}

TEST_F(ParserTest, DanglingOutputRedirectDoesNotCrash) {
    parse("ls >\n");
    EXPECT_EQ(cmd->output_file, nullptr);
}

TEST_F(ParserTest, TooManyArgsIsCapped) {
    std::string line = "cmd";
    for (int i = 0; i < MAX_ARGS + 10; i++) line += " x";
    line += "\n";
    parse(line.c_str());
    EXPECT_EQ(cmd->argc, MAX_ARGS);
    EXPECT_EQ(cmd->argv[MAX_ARGS], nullptr);
}
