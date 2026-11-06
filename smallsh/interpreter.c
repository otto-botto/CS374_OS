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
        if (WIFEXITED(state->last_fg_status)) {
            printf("exit value %d\n", WEXITSTATUS(state->last_fg_status));
        }else if (WIFSIGNALED(state->last_fg_status)) {
            printf("terminated by signal %d\n", WIFSIGNALED(state->last_fg_status));
        }else {
            printf("exit value %d\n", state->last_fg_status);
        }

    }
    else if (comm_number == EMPTY) {
        ;
    }

}

/* handle non-built in commands */
void run_other_comm(state_t *state, command_line_t *command) {
    pid_t spawnpid = -5;
    int child_status;

    spawnpid = fork();
    switch (spawnpid) {
        case -1: {
            perror("fork()");
            break;
        }
        case 0: {
            // this is the child
            // use execvp, p == reading PATH
            execvp(command->argv[0], command->argv);
            perror(command->argv[0]);
            exit(EXIT_FAILURE); // child has to return so parent can be unblocked
        }
        default: {
            // this is the parent
            // foreground, wait and add status
            if (command->is_bg == false) {
                waitpid(spawnpid, &child_status,0);
                state->last_fg_status = child_status;
            }else {
                // background, spawnpid is child's pid, add PID
                if (state->bg_pids_count < MAX_BG_PIDS) {
                    state->bg_pids[state->bg_pids_count] = spawnpid;
                    state->bg_pids_count++;
                }else {
                    printf("exceeded 100 PIDs\n");
                }

            }
            break;
        }
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
            run_other_comm(state, command);
            break;
    }

}