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
        }else {
            path = getenv("HOME");
            if (chdir(path) == -1) {
                perror("chdir(HOME)");
            }
        }
    }
    else if (comm_number == EXIT_BI) {
        state->keep_running = false;
    }
    else if (comm_number == STATUS_BI) {
        if (WIFEXITED(state->last_fg_status)) {
            printf("exit value %d\n", WEXITSTATUS(state->last_fg_status));
        }else if (WIFSIGNALED(state->last_fg_status)) {
            printf("terminated by signal %d\n", WTERMSIG(state->last_fg_status));
        }else {
            printf("exit value %d\n", state->last_fg_status);
        }

    }
    else if (comm_number == EMPTY) {
        ;
    }

}

/* open a file for writing for redirected output */
int redirect_output(char *file_name) {
    if (file_name == NULL) {
        return -1;
    }else {
        int file_descriptor;
        file_descriptor = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0640);
        if (file_descriptor == -1) {
            return -1;
        }
        if (fchmod(file_descriptor, 0640) == -1) {
            perror("chmod file");
            close(file_descriptor);
            return -1;
        }
        if (dup2(file_descriptor, 1) == -1) {
                close(file_descriptor); // opened and redirect failed, so close the file here
                return -1;
        }else {
                return file_descriptor;
        }

    }
}

/* open a file for reading for redirected input */
int redirect_input(char *file_name) {
    if (file_name == NULL) {
        return -1;
    }else {
        int file_descriptor;
        file_descriptor = open(file_name, O_RDONLY);
        if (file_descriptor == -1) {
            return -1;
        }else {
            // 0 stdin, 1 stdout, 2 stderr
            if (dup2(file_descriptor, 0) == -1) {
                return -1;
            }else {
                return file_descriptor;
            }
        }
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
            if (command->output_file != NULL) {
                int output = redirect_output(command->output_file);
                if (output == -1) {
                    printf("cannot open %s for output\n", command->output_file);
                    fflush(stdout);
                    exit(EXIT_FAILURE);
                }
                close(output);
            }
            if(command->input_file != NULL) {
                int input = redirect_input(command->input_file);
                if (input == -1) {
                    printf("cannot open %s for input\n", command->input_file);
                    fflush(stdout);
                    exit(EXIT_FAILURE);
                }
                close(input);
            }
            // no redirection
            // if background command, redirect input and output /dev/null
            if (command->is_bg == false) { // foreground child, terminate on interrupt
                struct sigaction SIGINT_action = {0};
                SIGINT_action.sa_handler = SIG_DFL; // default, which is terminate
                sigaction(SIGINT, &SIGINT_action, NULL); // a semi-custom handler
            }
            execvp(command->argv[0], command->argv);
            perror(command->argv[0]);
            exit(EXIT_FAILURE); // child has to return so parent can be unblocked
        }
        default: {
            // this is the parent
            // foreground, reap the child with wait and add status
            if (command->is_bg == false) {
                waitpid(spawnpid, &child_status,0);
                state->last_fg_status = child_status;
                if (child_status == 2) {
                    char message[] = "terminated by signal 2\n";
                    write(STDOUT_FILENO, message, sizeof(message) -1);
                }

            }else {
                // background, spawnpid is child's pid, add PID
                if (state->bg_pids_count < MAX_BG_PIDS) {
                    state->bg_pids[state->bg_pids_count] = spawnpid;
                    state->bg_pids_count++;
                    printf("background pid is %d\n", spawnpid);
                }else {
                    printf("exceeded 100 PIDs\n");
                }

            }
            break;
        }
    }
}

void check_bg_pids(state_t *state) {
    pid_t curr_pid = -5;
    int curr_pid_status;

    for (int  i  = 0; i < state->bg_pids_count; i++) {
        curr_pid = state->bg_pids[i];
        pid_t result = waitpid(curr_pid, &curr_pid_status, WNOHANG);
        if (result == curr_pid) { // waitpid returns the pid_t of the child process if it has finished and fills in the status
            if (WIFEXITED(curr_pid_status)) {
                printf("background pid %d is done: exit value %d\n", curr_pid, WEXITSTATUS(curr_pid_status));
                fflush(stdout);
            }else if (WIFSIGNALED(curr_pid_status)) {
                printf("background pid %d is done: terminated by signal %d\n", curr_pid, WTERMSIG(curr_pid_status));
                fflush(stdout);
            }
            // remove this process, put last one in, reduce count
            state->bg_pids[i] = state->bg_pids[state->bg_pids_count - 1]; // last entry in this slot
            state->bg_pids_count--;
            // now index i holds an unchecked process, decrement i to check this slot again
            i--;
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