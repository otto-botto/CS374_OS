#include <stdio.h>
#include "parser.h"
#include "interpreter.h"

int main(void) {
    state_t state;
    init_state(&state);

    while (state.keep_running) {
        if (state.bg_pids_count > 0) {
            check_bg_pids(&state);
        }
        command_line_t *command = parse_input();
        run_command(&state, command);
        free_command_line(command); // need to free every command
    }
    return 0;
}