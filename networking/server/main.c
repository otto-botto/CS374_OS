#include <stdio.h> // printf
#include <stdlib.h> // exit, strtol
#include <string.h> //memcpy
#include <errno.h> //errno and ERANGE
#include <netinet/in.h> // in_addr, sockaddr_in
#include <sys/socket.h> // socket(), connect(), bind(), etc.
#include <sys/types.h> //
#include <unistd.h> // read(), write(), close()
#include <asm-generic/errno-base.h>
#include <arpa/inet.h> // inet_ntoa

/* report errors*/
void error(const char *msg) {
    perror(msg);
    exit(1);
}

/* set up the server's own local address: which IP and which port
 * sockaddr_in is a socket address, Internet IPv4
 */
struct sockaddr_in setupAddressStruct(int portNumber) {
    struct sockaddr_in address = {
        .sin_family = AF_INET,                          // family is IPv4
        .sin_port = htons(portNumber),                  // port, host -> network short
        .sin_addr.s_addr = htonl(INADDR_ANY),           // ip address, host -> network long
    };
    return address;
}

int main(int argc, char **argv) {
    int connectionSocket, charsRead;
    char buffer[256];
    socklen_t sizeOfClientInfo = sizeof(struct sockaddr_in);
    long parsed;
    char *end;
    struct sockaddr_in clientAddress;   // filled in by accept()

    //check usage and args
    if (argc < 2) {
        fprintf(stderr, "USAGE: %s port.\n", argv[0]);
        exit(1);
    }

    // create a socket that will listen for connections
    int listenSocket = socket(AF_INET, SOCK_STREAM,0);
    if (listenSocket < 0) {
        error("ERROR opening socket.\n");
    }

    // set up the address struct for the server socket
    // this just parses the argument given to the function, like "8080"
    errno = 0;
    parsed = strtol(argv[1], &end, 10);
    if (end == argv[1] || *end != '\0' || errno == ERANGE || parsed < 1 || parsed > 65535) {
        fprintf(stderr, "Invalid port %s\n", argv[1]);
    }
    int portNumber = (int) parsed;
    struct sockaddr_in serverAddress = setupAddressStruct(portNumber);

    // associate the socket to the port using bind()
    if (bind(listenSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        error("ERROR on binding");
    }

    // start listening for connections, allow up to 5 conn to queue up
    listen(listenSocket, 5);

    // accept a connection
    while (1) {
        // accept a conn request, this creates a socket
        connectionSocket = accept(listenSocket, (struct sockaddr *)&clientAddress, &sizeOfClientInfo);
        if (connectionSocket < 0) {
            error("ERROR on accept");
        }

        printf("SERVER: Connected to client running at host %s port %d\n",
             inet_ntoa(clientAddress.sin_addr),
             ntohs(clientAddress.sin_port));
        // get the message from the client and display it
        memset(buffer, '\0', 256);
        // read the client's message from the socket
        charsRead = recv(connectionSocket, buffer, 255, 0);
        if (charsRead < 0) {
            error("ERROR reading from socket");
        }
        printf("SERVER: I received this from the client: \"%s\"\n", buffer);

        // send a success message back to the client
        charsRead = send(connectionSocket, "I am the server, and I got your message", 39,0);
        if (charsRead < 0) {
            error("ERROR writing to socket");
        }
        // close the connection socket for this client
        close(connectionSocket);
    }
    // close the listening socket
    close(listenSocket);

    return 0;
}