#include <stdio.h>
#include "parser.h"
#include "interpreter.h"

int main(void) {
    state_t state;
    init_state(&state);

    while (state.keep_running) {
        command_line_t *command = parse_input();
        run_command(&state, command);
    }

    return 0;
}