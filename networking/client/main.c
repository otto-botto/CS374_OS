#include <stdio.h>
#include <stdio.h> // printf
#include <stdlib.h> // exit, strtol
#include <string.h> //memcpy
#include <errno.h> //errno and ERANGE
#include <netinet/in.h> // in_addr, sockaddr_in
#include <sys/socket.h> // socket(), connect(), bind(), etc.
#include <sys/types.h> //
#include <netdb.h> // gethostbyname()
#include <unistd.h> // read(), write(), close()
#include <asm-generic/errno-base.h>
#include <arpa/inet.h> // inet_nto

/* report errors */
void error(const char *msg) {
    perror(msg);
    exit(1);
}

/* set up the client's address struct */
struct sockaddr_in setupAddressStruct(int portNumber, char *hostName) {
    // get the DNS entry for this host name
    struct hostent *hostInfo = gethostbyname(hostName);
    if (hostInfo == NULL || hostInfo->h_addrtype != AF_INET) {
        fprintf(stderr, "CLIENT: ERROR, no such host\n");
        exit(1);
    }

    // set up the fields with ready values: family and port
    struct sockaddr_in address = {
        .sin_family = AF_INET,
        .sin_port = htons(portNumber),
    };
    // copy the server's IP from the DNS entry (already network order in h_addr_list)
    // declaring address gives me 16 bytes, memcpy writes 4 through 8
    memcpy(&address.sin_addr, hostInfo->h_addr_list[0], sizeof(address.sin_addr));

    return address;
}

int main(int argc, char **argv) {
    int connectionSocket, portNumber;
    ssize_t charsSent, charsReceived;
    char buffer[256];

    long parsed;
    char *end;

    // check usage and args
    if (argc < 3) {
        fprintf(stderr, "USAGE: %s hostname port\n", argv[0]);
    }

    // create a socket file descriptor
    connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (connectionSocket < 0) {
        error("ERROR opening socket");
    }

    // set up the socket address

    // 1. get input and convert to network order port
    errno = 0;
    parsed = strtol(argv[1], &end, 10);
    if (end == argv[1] || *end != '\0' || errno == ERANGE || parsed < 1 || parsed > 65535) {
        fprintf(stderr, "Invalid port %s\n", argv[1]);
    }
    portNumber = (int)parsed;
    // 2. pass the port number and host name
    struct sockaddr_in clientAddress = setupAddressStruct(portNumber, argv[2]); // can pass argv[2] directly, beucase if too long gethosbyname becomes NULL

    // connect to a listening server
    if (connect(connectionSocket, (struct sockaddr *)&clientAddress, sizeof(clientAddress)) < 0) {
        error("CLIENT: ERROR connecting");
    }

    // write and read

    // 1. get input message from the user
    printf("CLIENT: enter text to send to the server, and then hit enter: ");
    fflush(stdout); // make sure the prompt comes before waiting for the input

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        fprintf(stderr, "CLIENT: no input\n");
        exit(1);
    }
    buffer[strcspn(buffer, "\n")] = '\0'; // remove trailing new line if any

    // 2. write to server
    charsSent = send(connectionSocket, buffer, strlen(buffer), 0);
    if (charsSent < 0) {
        error("CLIENT: ERROR sending to socket");
    }

    // 3. get return message from server
    charsReceived = recv(connectionSocket, buffer, sizeof(buffer) -1, 0); // -1, room for terminator
    if (charsReceived < 0) {
        error("CLIENT: ERROR receiving from socket");
    }

    buffer[charsReceived] = '\0'; // recv does not write the \0 in the end
    printf("CLIENT: I received this from the server \"%s\"\n", buffer);

    // close the socket
    close(connectionSocket);
    return 0;
}