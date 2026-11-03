#include <stdio.h> // for perror
#include <unistd.h> // for execv
#include <sys/types.h>
#include <sys/wait.h> // for waitpid
#include <stdlib.h> // for exit

// int forking(void) {
//     pid_t spawnpid = -1;
//     int intVal = 10;
//     // if fork is successful, spawnid will be 0 in the child and the child's id in the parent
//     spawnpid = fork();
//     switch (spawnpid) {
//         case -1: {
//             perror("fork() failed");
//             exit(1);
//             break;
//         }
//         case 0: {
//             // spawnpid is 0 in the child, success
//             intVal = intVal + 1;
//             printf("I am the child! intVal = %d\n", intVal);
//             break;
//         }
//         default: {
//             // spawnpid is the pid of the child
//             intVal -= 1;
//             printf("I am the parent! intVal = %d\n", intVal);
//
//         }
//     }
//     printf("This statement will be executed by both of us.\n");
//     return spawnpid;
// }
//
// int waiting(void) {
//     pid_t spawnpid = -5;
//     int childStatus;
//     int childPid;
//
//     spawnpid = fork();
//     switch (spawnpid) {
//         case -1: {
//             perror("fork() failed!");
//             //exit(EXIT_FAILURE);
//             return 1;
//             break;
//         }
//         case 0: {
//             // spawnpid is 0 in the child
//             printf("I am the child. My pid is %d\n", getpid());
//         }
//         default: {
//             // spawnpid is the pid of the child
//             printf("I am the parent. My pid is %d\n", getpid());
//             childPid = wait(&childStatus);
//             printf("Parent is done waiting as the child with pid %d exited\n", childPid);
//             break;
//         }
//     }
//     printf("The process with pid %d is returning from waiting().\n");
//     // return EXIT_SUCCESS;
//     return 0;
// }

// int main(int argc, char **argv) {
//     int childOneStatus;
//     int childTwoStatus;
//     printf("Parent process's pid = %d\n", getpid());
//     pid_t firstChild = fork();
//
//     if (firstChild == -1) {
//         perror("fork() failed.");
//         exit(EXIT_FAILURE);
//     }else if (firstChild == 0) {
//         // the first child process will execute this branch
//         printf("First child's pid = %d\n", getpid());
//         sleep(5);
//     }else {
//         // the parent executes this branch
//         // fork another child
//         pid_t secondChild = fork();
//         if (secondChild == -1) {
//             perror("fork() second child faield.");
//             waitpid(firstChild, &childOneStatus, 0);
//             exit(EXIT_FAILURE);
//         }else if (secondChild == 0) {
//             printf("Second child pid = %d\n", getpid());
//             sleep(10);
//         }else {
//             // the parent executes this branch
//             // wait for the second child
//             pid_t childTwoPid = waitpid(secondChild, &childTwoStatus, 0);
//             pid_t childOnePid = waitpid(firstChild, &childOneStatus, 0);
//             printf("The parent is done waiting for second child. The pid child that terminated is %d\n", childTwoPid);
//             printf("The parent is done waiting for first child. The pid child that terminated is %d\n", childOnePid);
//             if (WIFEXITED(childOneStatus)) {
//                 printf("Child one %d exited normally.\n", childOnePid);
//                 if (WIFEXITED(childTwoStatus)) {
//                     printf("Child two %d exited normally.\n", childTwoPid);
//                 }
//             }else if (WIFEXITED(childTwoStatus)) {
//                 printf("Child two %d exited normally.\n", childTwoPid);
//                 if (WIFEXITED(childOneStatus)) {
//                     printf("Child one %d exited normally.\n", childOnePid);
//                 }
//             }else {
//                 printf("Neither child %d nor %d exited normally.\n", childOnePid, childTwoPid);
//             }
//         }
//     }
//     printf("The process with pid %d is exiting.\n", getpid());
//     return EXIT_SUCCESS;
// }

/**
 * Execute the command passed in argv[1]
 * Pass the value in argv[2] as an argumentto this command.
 */
int main(int argc, char **argv) {
    if (argc < 5) {
        printf("You must provide the command and one argument for the command.\n");
        printf("For example: ./my_shell /bin/ls -la\n");
        return EXIT_FAILURE;
    }
    char *lsArgv[] = {argv[1], argv[2], NULL};
    char *catArgv[] = {argv[3], argv[4], NULL};
    int childOneStatus;
    int childTwoStatus;

    // for a new process
    pid_t childOnePid = fork();
    switch (childOnePid) {
        case -1: {
            perror("fork() child one\n");
            exit(1);
            break;
        }
        case 0:
        {
            // the child process executes this branch
            printf("CHILD %d running ls command\n", getpid());
            // replace the current program with "bin/ls"
            execv(lsArgv[0], lsArgv);
            // execv only returns if there is an error
            perror("execv");
            exit(2);
            break;
        }
        default: {
            // the parent process executes this branch
            // create the second child
            pid_t childTwoPid = fork();
            if (childTwoPid == -1) {
                perror("fork() child two");
                exit(3);
            }else if (childTwoPid == 0) {
                // second child executes this branch
                printf("CHILD %d running cat command\n", getpid());
                // replace the current process with "bin/cat"
                execv(catArgv[0], catArgv);
                // execv only returns on error
                perror("execv");
                exit(4);
            }
            // wait for the child's termination
            childOnePid = waitpid(childOnePid, &childOneStatus, 0);
            childTwoPid = waitpid(childTwoPid, &childTwoStatus, 0);
            printf("PARENT (%d): child(%d) and child (%d) both terminated. Now parent is exiting.\n", getpid(), childOnePid, childTwoPid);
            exit(0);
            break;
        }
    }
}