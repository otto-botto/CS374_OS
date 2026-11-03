#include <gtest/gtest.h>

extern "C"{
#include "interpreter.h"
}

TEST(Hello, HelloTest) {
    int result = hello();
    EXPECT_EQ(3, result);
}

TEST(CommLookUp, CommLookUpTest) {
    char comm_exit[]    = "exit";
    char comm_cd[]    = "cd";
    char comm_status[]    = "status";
    char comm_unknown[]    = "lls";

    int command_exit = command_lookup(comm_exit);
    EXPECT_EQ(command_exit, 0);

    int command_cd = command_lookup(comm_cd);
    EXPECT_EQ(command_cd, 1);

    int command_status = command_lookup(comm_status);
    EXPECT_EQ(command_status, 2);

    int command_unknown = command_lookup(comm_unknown);
    EXPECT_EQ(command_unknown, -1);
}