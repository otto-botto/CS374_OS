#include <stdio.h> // printf
#include <stdlib.h> // exit
#include <string.h> // memset
#include <errno.h> // errno set by strtol and ERANGE
#include <netdb.h> // hostent
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // inet_ntoa

/**
* Set up a sockaddr_in structure with the value of hostname and port passed as arguments to the program
*/

int main(int argc, char *argv[]) {
    int portNumber;
    struct hostent *hostInfo;
    long parsed;
    char *end;
    // struct sockaddr_in address;

    // check usage and args
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s hostname port\n", argv[0]);
        fprintf(stderr, "For example: %s oregonstate.edu 8080\n", argv[0]);
        exit(0);
    }

    // store the port number
    errno = 0; // have to clear
    parsed = strtol(argv[2], &end, 10);
    if (end == argv[2] || *end != '\0' || // "abc" "" or "80abc" "80 "
        errno == ERANGE || // too large for a long to hold
        parsed < 1 || parsed > 65535) { // not within the range for a valid port number
        fprintf(stderr, "Invalid port: %s\n", argv[2]);
    }
    portNumber = (int) parsed;



    // a cleaner way to set up the initializer for socket address:
    struct sockaddr_in address = {
        .sin_family = AF_INET, // network capable for IPv4
        .sin_port = htons(portNumber),
    };

    /* setting up the socket address */
    // clear out the address struct
    // memset(&address, 0, sizeof(address));
    // the address should be network-capable
    //address.sin_family = AF_INET; // for IPv4
    //address.sin_port = htons(portNumber);

    // get the DNS entry for this host name
    hostInfo = gethostbyname(argv[1]);
    if (hostInfo == NULL) {
        fprintf(stderr, "CLIENT: ERROR, no such host\n");
        exit(0);
    }

    // before copying from hostInfo, ensure AF_NET, because .sin_family has room only for 4 bytes
    if (hostInfo->h_addrtype != AF_INET) {
        fprintf(stderr,"Socket set for IP4v, but host sent other format.\n");
    }

    // copy the first IP address from the DNS entry to sin_addr.s_addr
    memcpy(&address.sin_addr, hostInfo->h_addr_list[0], hostInfo->h_length);

    // print the IP address in the socket address as a dotted decimal string
    printf("IP address for %s: %s\n", argv[1], inet_ntoa(address.sin_addr));

    // print the port number in the socket address in the variable address
    printf("Port number in the socket address: %d\n", ntohs(address.sin_port));


    return 0;
}