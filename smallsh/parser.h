//
// Created by lora on 11/2/26.
//

#ifndef SMALLSH_PARSER_H
#define SMALLSH_PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define INPUT_LENGTH 2048
#define MAX_ARGS 512

typedef struct CommandLine{
    char *argv[MAX_ARGS + 1];
    int argc;
    char *input_file;
    char *output_file;
    bool is_bg;
}command_line_t;

command_line_t *parse_input(void);
command_line_t *parse_line(char *line);
void free_command_line(command_line_t *command);

#endif //SMALLSH_PARSER_H