//
// Created by lora on 11/3/26.
//
#ifndef SMALLSH_INTERPRETER_H
#define SMALLSH_INTERPRETER_H

/* I have taken the boot.dev course on memory management which taught me about enums
 * I have also tried to follow the book Crafting Interpreters in the past and am
 * drawing some of the structure of my program from Robert Nystrom's book: https://craftinginterpreters.com/
 */

#include <string.h>

typedef enum {
    EXIT_BI,
    CD_BI,
    STATUS_BI,
}command_t;

int hello(void);
int command_lookup(char *command);
void run_command(command_t command);
#endif //SMALLSH_INTERPRETER_H