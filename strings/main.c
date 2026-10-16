#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    // Data Segment allocation
    char *dataSegString = "char * means string in Data Segment, cannot change";
    printf("%s\n", dataSegString);
    // dataSegString[2] = 'e'; won't work, because read-only data segment of the stack

    // Stack allocation
    char stackString[] = "stack String";
    char stackString2[13]=  "stack String";
    printf("%s\n", stackString);
    printf("%s\n", stackString2);

    // String Functions

    // string length ==> strlen() returns length, excl. \0
    size_t l1 = strlen(stackString);
    size_t l2 = strlen(stackString2);
    printf("stackString is %lu characters long\n", l1);
    printf("stackString2 is %lu characters long\n", l2);

    // compare strings ==> strcmp, returns int, - if lhs bigger, + if rhs bigger, 0 for equal
    int compare = strcmp(stackString, stackString2);
    int compareNine = strncmp(dataSegString, stackString, 9);
    printf("compare: %d\n", compare);
    printf("compare first 9 chars: %d\n", compareNine);

    // copy string ==> strcpy returns char* of destination string, includes \0
    char *myString3 = calloc(strlen(dataSegString)+1, sizeof(char));
    strcpy(myString3, dataSegString);
    printf("%s\n", myString3);
    free(myString3);

    // concatenate two strings ==> strcat, returns char* of destination string, includes \0
    char* concatString = calloc(strlen(dataSegString) + strlen(stackString) + 1, sizeof(char));
    strcpy(concatString, dataSegString);
    strcat(concatString, stackString);
    printf("%s\n", concatString);


    // duplicate a string ==> strdup, allocated memory with malloc and returns pointer to new duplicate string
    char *duplicateString = strdup(concatString);
    printf("diplicate string  is %lu chars long: %s\n", strlen(duplicateString), duplicateString);
    free(duplicateString);

    free(concatString);

    // tokenize a string ==> strtok_r, returns char * to a string
    char ips[] = "127.0.0.1, 192.0.2.1, 172.16.254.1";
    // best to copy the string, so we don't modify it
    char *ips_dup = strdup(ips);
    char *saveptr1;

    char *store_tokens[10];
    int token_counter = 0;
    char *tok = strtok_r(ips_dup, ", ", &saveptr1); // this tokenizes only up to the first ", "

    while (tok != NULL && token_counter < 10) {
        printf("tok: %s\n", tok);
        // cannot use strcat here, because that throws away the '\0' which delimits the tokens
        store_tokens[token_counter] = tok;
        tok = strtok_r(NULL, ", ", &saveptr1);
        token_counter++;
    }
    printf("tok: %s\n", tok);
    for (int i = 0; i < token_counter; i++) {
        printf("token %d: %s\n", i+1, store_tokens[i]);
    }
    printf("ips: %s\n", ips);
    printf("ips_dup: %s\n", ips_dup);
    free(ips_dup);
    return 0;

}
