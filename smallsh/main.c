#include <stdio.h>
#include "parser.h"
#include "interpreter.h"

int main(void) {
    while (1) {
        command_line_t *command = parse_input();
        printf("Command: %s and argc: %d\n", command->argv[0], command->argc);
        char *com = command->argv[0];
        switch () {

        }
    }
    return 0;
}