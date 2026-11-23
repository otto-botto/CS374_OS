#ifndef SMALLSH_INTERPRETER_H
#define SMALLSH_INTERPRETER_H

/* I have taken the boot.dev course on memory management which taught me about enums
 * I have also tried to follow the book Crafting Interpreters, and I am
 * drawing some of the structure of my program from the book: https://craftinginterpreters.com/
 */

#include <string.h>
#include <unistd.h> // for execv, dup2
#include <sys/types.h>
#include <sys/stat.h> // for fchmod
#include <sys/wait.h> // for waitpid
#include <stdio.h> // for perror
#include <stdlib.h> // for exit and getenv() to get home dir
#include <fcntl.h> // for open, write, read files
#include <stdbool.h>
#include "parser.h"

#define MAX_BG_PIDS 100

/* shell state */
typedef struct State {
    int last_fg_status;
    pid_t bg_pids[MAX_BG_PIDS];
    int bg_pids_count;
    bool keep_running;
}state_t;

/* build-in (BI) commands */
typedef enum {
    EMPTY,
    EXIT_BI,
    CD_BI,
    STATUS_BI,
    EXTERNAL,
}command_t;

void init_state(state_t *state);
void check_bg_pids(state_t *state);
command_t command_lookup(char *command);
void run_command(state_t *state, command_line_t *command);
void run_built_in_comm(state_t *state, command_t comm_number, command_line_t *command);
void run_other_comm( state_t *state, command_line_t *command);
int redirect_output(char *file_name);
int redirect_input(char *file_name);
void handle_SIGTSTP(int signo);

#endif //SMALLSH_INTERPRETER_H