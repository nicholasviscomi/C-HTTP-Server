#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "Colors.h"
#include "File.h"
#include "String.h"

#define PORT 8080

int initSocket(int domain, int type, int protocol) {
    int sock;
    if ((sock = socket(domain, type, protocol)) < 0) {
        printf("error creating socket");
        exit(EXIT_FAILURE);
    }
    return sock;
}

void bindSocket(int socket, struct sockaddr_in *address, socklen_t len) {
    if (bind(socket, (struct sockaddr *)address, len) < 0) {
        printf("Binding error");
        exit(EXIT_FAILURE);
    }
    return;
}

const int BUFFSIZE = 30000;
char buffer[BUFFSIZE]; //buffer for reading from socket
void read_from_socket(int socket, void *buff, const int size) {
    read(socket, buff, size);
    setTerminalColor(WHITE);
    printf("%s\n",buffer);
    return;
}

void write_to_socket(int socket, char *msg, size_t len) {
    write(socket, msg, len);
    setTerminalColor(CYAN);
    printf("------------------message sent-------------------\n");
    return;
}

void run_server() {
    int server; //server_fd: _fd = file descriptor
    // long valread;
    
    //Step 1: Create Socket
    server = initSocket(AF_INET, SOCK_STREAM, 0);  //I think in java terms this is a server socket

    //Step 2: Assign that socket to a port (bind) 
    struct sockaddr_in server_addr; //socket address internet: sockaddr is a generic type while sockaddr_in is more specific
    int addrlen = sizeof(server_addr);

    memset((char *)&server_addr, '\0', addrlen); //I think its just setting everything else to zero
    server_addr.sin_family = AF_INET; //address family used to setup the socket
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT); //port number; host to network short (htons)
    
    bindSocket(server, &server_addr, addrlen);

    //Step 3: Wait for incoming connections
    //param 2: backlog = the max length the queue of pending connections can get
    if (listen(server, 10) < 0) { printf("Error listening"); exit(EXIT_FAILURE); }

    int client;
    while(1) {
        setTerminalColor(YELLOW); 
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        
        if ((client = accept(server, (struct sockaddr *)&server_addr, (socklen_t*)&addrlen)) < 0) {
            printf("Error accepting");
            exit(EXIT_FAILURE);
        }

        read_from_socket(client, buffer, BUFFSIZE); //the data that is read lives in char buffer[BUFFSIZE]
        
        char *response = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
        char *contents = fileContents("/Users/nickviscomi/Desktop/VSCode/C/HTTPServer/src/Web/index.html");
      
        char* contLen = malloc(20); 
        sprintf(contLen, "%lu", strlen(contents));
        response = concatenate(response, contLen); //add content length
        
        contents = concatenate("\n\n", contents); //add two new lines to format response correctly
        response = concatenate(response, contents); //add contents

        printf("=========================== Response ================================\n");
        printf("%s", response);

        write_to_socket(client, response, strlen(response));

        setTerminalColor(CYAN);
        printf("------------------message sent-------------------\n");

        close(client);
    }
}

int main(int argc, char const *argv[]) {
    run_server();
    return 0;
}






