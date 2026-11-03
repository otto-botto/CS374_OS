//
// Created by lora on 11/3/26.
//
#include "interpreter.h"


int hello(void) {
    return 3;
}

int command_lookup(char *command) {
    if (strcmp(command, "exit") == 0) {
        return EXIT_BI;
    }else if (strcmp(command, "cd") == 0) {
        return CD_BI;
    }else if (strcmp(command, "status") == 0) {
        return STATUS_BI;
    }else {
        return -1;
    }
}

void run_command(command_t command) {
    switch (command) {
        case EXIT_BI: {

            break;
        }

    }
}