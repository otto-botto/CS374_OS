#include <gtest/gtest.h>

extern "C"{
#include "interpreter.h"
}

class InterpreterTest : public ::testing::Test {
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

TEST_F(InterpreterTest, EmptyCommand) {
    parse("\n");
    int comm_lookup = command_lookup(cmd->argv[0]);
    EXPECT_EQ(cmd->argc, 0);
    EXPECT_EQ(cmd->argv[0], nullptr);
    EXPECT_EQ(comm_lookup, EMPTY);
}

TEST_F(InterpreterTest, UnknownCommand) {
    parse("ls\n");
    EXPECT_EQ(cmd->argc, 1);
    EXPECT_STREQ(cmd->argv[0], "ls");
    EXPECT_EQ(cmd->argv[1], nullptr);
    EXPECT_EQ(cmd->input_file, nullptr);
    EXPECT_EQ(cmd->output_file, nullptr);
    EXPECT_EQ(cmd->argv[2], nullptr);
    EXPECT_FALSE(cmd->is_bg);

    int comm_lookup = command_lookup(cmd->argv[0]);
    EXPECT_EQ(comm_lookup, UNKNOWN);
}

TEST(InitState, InitStateTest) {
    state_t state;
    init_state(&state);
    EXPECT_EQ(true, state.keep_running);
    EXPECT_EQ(0, state.bg_pids_count);
    EXPECT_EQ(0, state.last_fg_status);

}

TEST(CommLookUp, CommLookUpTest) {
    char comm_exit[]    = "exit";
    char comm_cd[]    = "cd";
    char comm_status[]    = "status";
    char comm_unknown[]    = "lls";

    int command_exit = command_lookup(comm_exit);
    EXPECT_EQ(command_exit, 1);

    int command_cd = command_lookup(comm_cd);
    EXPECT_EQ(command_cd, 2);

    int command_status = command_lookup(comm_status);
    EXPECT_EQ(command_status, 3);

    int command_empty = command_lookup(nullptr);
    EXPECT_EQ(command_empty, 4);

    int command_unknown = command_lookup(comm_unknown);
    EXPECT_EQ(command_unknown, 5);
}

