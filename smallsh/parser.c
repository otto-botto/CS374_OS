#include "parser.h"

/* actual parsing fn, returns a struct of arguments and flags
 * used the parsing starter code provided in the assignment
 */
command_line_t *parse_line(char *line) {
    command_line_t *curr_command = calloc(1,sizeof(command_line_t));
    // Tokenize the input
    char *token = strtok(line, " \n");
    while(token){
        if(!strcmp(token,"<")){
            // guard if the user enters "cat < " and forgets to give a file
            char *given_file = strtok(NULL, " \n");
            if (given_file) {
                curr_command->input_file = strdup(given_file); // strdup calls malloc, so needs release
            }

        } else if(!strcmp(token,">")){
            char *given_file = strtok(NULL, " \n");
            if (given_file) {
                curr_command->output_file = strdup(given_file); // strdup calls malloc, so needs release
            }

        } else if(!strcmp(token,"&")){
            curr_command->is_bg = true;
        } else{
            // guard that only up to MAX is entered
            if (curr_command->argc < MAX_ARGS) {
                curr_command->argv[curr_command->argc++] = strdup(token); // strdup calls malloc, so needs release
            }
        }
        token=strtok(NULL," \n");
    }

    return curr_command;
}

/* main parsing fn, prompts input and passes it to parse line helper */
command_line_t *parse_input(void) {
    char input[INPUT_LENGTH];

    // Get input
    printf(": ");
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);

    return parse_line(input);
}

/* free memory allocated for the command */
void free_command_line(command_line_t *command) {
    int num_arguments = command->argc;
    // first free the argv, an array of strings
    for (int i = 0; i < num_arguments; i++) {
        free(command->argv[i]);
    }
    // free input and output file strings
    free(command->input_file);
    free(command->output_file);
    //free the struct
    free(command);
}