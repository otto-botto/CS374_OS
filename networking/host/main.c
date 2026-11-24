#include <stdio.h> // printf, stderr
#include <stdlib.h> // exit
#include <netdb.h> // network database: hostent, gethostname
#include <string.h> // memcpy
#include <netinet/in.h> // network internet: in_addr
#include <arpa/inet.h> // conversions: inet_ntoa



int main(int argc, char **argv) {
    // check usage and args
    if (argc != 2) {
        fprintf(stderr, "Usage: %s $hostname \n", argv[0]);
        fprintf(stderr, "Example: %s flip.engr.oregonstate.edu\n", argv[0]);
        exit(0);
    }

    // get DNS entry associated with the domain name
    struct hostent *hostptr = gethostbyname(argv[1]);
    if (hostptr == NULL) {
        printf("Couldn't locate %s\n", argv[1]);
        exit(0);
    }

    // we successfully looked up the DNS entry
    printf("Official domain name: %s \n", hostptr->h_name);

    // print all aliaces of the domain name
    int  alias_counter = 0;
    printf("Aliaces: \n");
    while (hostptr->h_aliases[ alias_counter] != NULL) {
        printf("\t%s\n", hostptr->h_aliases[ alias_counter]);
         alias_counter++;
    }

    // print all IP addresses, in dotted decimal notation
    int ip_counter = 0;
    printf("Addresses: \n");
    struct in_addr addr;
    while (hostptr->h_addr_list[ip_counter] != NULL) {
        // h_addr_list is char **h_addr_list ptr to char ptrs, ending with NULL
        // addr.s_addr = *(unsigned int *)hostptr->h_addr_list[ip_counter];
        // safer way, with memcpy, which takes *void but needs the length, 4 bytes
        memcpy(&addr.s_addr, hostptr->h_addr_list[ip_counter], hostptr->h_length);
        printf("\t%s\n", inet_ntoa(addr));
        ip_counter++;
    }
    return 0;
}