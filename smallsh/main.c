#include <stdio.h>
#include <signal.h>
#include "parser.h"
#include "interpreter.h"

int main(void) {
    state_t state;
    init_state(&state);

    // if ctrl-C comes, ignore it
    struct sigaction SIGINT_action = {0};
    SIGINT_action.sa_handler = SIG_IGN;
    sigaction(SIGINT, &SIGINT_action, NULL); // a semi-custom handler


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