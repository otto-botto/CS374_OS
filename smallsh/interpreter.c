//
// Created by lora on 11/3/26.
//
#include "interpreter.h"

/* make a state for the shell */
void init_state(state_t *state) {
    state->last_fg_status = 0;
    state->bg_pids_count = 0;
    state->keep_running = true;
}

/* associated command with enum, so can call switch () */
command_t command_lookup(char *command) {
    if (command) {
        if (strcmp(command, "exit") == 0) {
            return EXIT_BI;
        }else if (strcmp(command, "cd") == 0) {
            return CD_BI;
        }else if (strcmp(command, "status") == 0) {
            return STATUS_BI;
        }else if (command[0] == '#') {
            return EMPTY;
        }
        else {
            return EXTERNAL;
        }
    }else {
        return EMPTY;
    }
}

/* handle built-in commands, empty and comment */
void run_built_in_comm(state_t *state, command_t comm_number, command_line_t *command) {
    if (comm_number == CD_BI) {
        char *path;
        // cd to a specific directory
        if (command->argc > 1) {
            path = command->argv[1];
            if (chdir(path) == -1) {
                perror("chdir(path)");
            }
            // TODO: remove print after pwd done
            char curr_dir[100];
            printf("%s\n", getcwd(curr_dir, sizeof(curr_dir)));
        }else {
            path = getenv("HOME");
            if (chdir(path) == -1) {
                perror("chdir(HOME)");
            }
            // TODO: remove print after pwd done
            char curr_dir[100];
            printf("%s\n", getcwd(curr_dir, sizeof(curr_dir)));
        }
    }
    else if (comm_number == EXIT_BI) {
        state->keep_running = false;
    }
    else if (comm_number == STATUS_BI) {
        printf("%d\n", state->last_fg_status);
    }
    else if (comm_number == EMPTY) {
        ;
    }

}

void run_command(state_t *state, command_line_t *command) {
    command_t comm_number = command_lookup(command->argv[0]);
    switch (comm_number) {
        case EXIT_BI:
        case STATUS_BI:
        case CD_BI:
        case EMPTY:
            run_built_in_comm(state, comm_number, command);
            break;
        default:
            printf("Unknown command.\n");
            break;
    }

}