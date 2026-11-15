#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

/* the signal handler for SIGINT */
void handle_SIGINT(int signo) {
    char message[] = "Caught SIGINT, sleeping for 10 seconds.\n";
    // using write rather than printf
    // printf is not signal safe to call in a handler, write is
    write(STDOUT_FILENO, message, sizeof(message) - 1); // -1 skips '\0'
    // in the second version, we raise SIGUSR2
    raise(SIGUSR2);
    sleep(10);
}

/* handler for SIGUSR2 */
void handle_SIGUSR2(int signo) {
    char message[] = "Caught SIGUSR2, exiting.\n";
    write(STDOUT_FILENO, message, sizeof(message) - 1);
    exit(0); // terminates the program
}


int main(void) {

    // initialize the SIGINT_action struct to be empty
    struct sigaction SIGINT_action = {0}, SIGUSR2_action = {0}, ignore_action = {0};

    // fill out the SIGINT_action struct
    // register hangle_SIGINT as the signal handler
    SIGINT_action.sa_handler = handle_SIGINT; // giving a pointer to the function storing in sa_handler
    // block all catchable signals while handle_SIGINT is running
    sigfillset(&SIGINT_action.sa_mask);
    // no flags are set
    SIGINT_action.sa_flags = 0;
    // install the custom signal handler
    sigaction(SIGINT, &SIGINT_action,NULL);

    // same process for SIGUSR2: set handler, block catchable, set flag
    SIGUSR2_action.sa_handler = handle_SIGUSR2;
    sigfillset(&SIGUSR2_action.sa_mask);
    SIGUSR2_action.sa_flags = 0;
    // register the custom signal handler for SIGUSR2
    sigaction(SIGUSR2, &SIGUSR2_action,NULL);

    // set the ignore_action struct to use SIG_IGN as its signal handler
    ignore_action.sa_handler = SIG_IGN;
    // register the SIG_IGN handler for SIGTERM, SIGHUP, SIGQUIT
    sigaction(SIGTERM, &ignore_action, NULL);
    sigaction(SIGHUP, &ignore_action, NULL);
    sigaction(SIGQUIT, &ignore_action, NULL);

    pid_t pid = getpid();
    printf("\nThis PID of this process is %d\n"
          "Send signals to the process using the kill command, e.g., kill -SIGTERM %d\n"
          "SIGTERM, SIGHUP, and SIGQUIT are disabled, so will be ignored.\n"
          "SIGUSR2 signal will kill this program.\n"
          "SIGINT signal will cause the program to sleep 10 seconds\n"
          "After 10 seconds the SIGINT handler will raise SIGUSR2 and the program will be killed.\n", pid, pid);

    while (1) {
        pause();
    }
    //pause(); // causes the process to pause until it is sent a signal
    // if pause was ended by SIGINT, then after handle_SIGINT ends,
    // the execution will proceed to this line

    return 0;
}