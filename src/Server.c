#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "Colors.h"
#include "File.h"
#include "String.h"
#include "json.h"

#define PORT 8080
char *HTTPSuccess = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
char *HTTPError = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
char *ConnectionsReq = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ";

char *FileStarter = "/Users/nickviscomi/Desktop/VSCode/C/HTTPServer/src/Web";
char *FileNotFoundPath = "/Users/nickviscomi/Desktop/VSCode/C/HTTPServer/src/Web/404.html";
const char *connectionsPath = "/Users/nickviscomi/Desktop/VSCode/C/HTTPServer/src/Database/Connection.json";

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
    return;
}

void write_to_socket(int socket, char *msg, size_t len) {
    write(socket, msg, len);
    setTerminalColor(CYAN);
    printf("\n------------------message sent-------------------\n");
    return;
}

void run_server() {
    int server; //server_fd: _fd = file descriptor
    // long valread;
    
    //Step 1: Create Socket
    server = initSocket(AF_INET, SOCK_STREAM, 0);  //I think in java terms this is a server socket

    //Step 2: Assign that socket to a port (bind) 
    struct sockaddr_in address; //will eventually be filled out with client informations 
    int addrlen = sizeof(address);

    memset((char *) &address, '\0', addrlen); //I think its just setting everything else to zero
    address.sin_family = AF_INET; //address family used to setup the socket
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT); //port number; host to network short (htons)
    
    bindSocket(server, &address, addrlen);

    //Step 3: Wait for incoming connections
    //param 2: backlog = the max length the queue of pending connections can get
    if (listen(server, 10) < 0) { printf("Error listening"); exit(EXIT_FAILURE); }

    int client;
    while(1) {
        setTerminalColor(YELLOW); 
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        
        if ((client = accept(server, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            printf("Error accepting");
            exit(EXIT_FAILURE);
        }

        read_from_socket(client, buffer, BUFFSIZE); //the data that is read lives in char buffer[BUFFSIZE]
        
        //--------draft and send response----------
        char *query = parseQuery(buffer);

        char *ipBuffer; 
        ipBuffer = inet_ntoa(address.sin_addr);
        int clientPort = ntohs(address.sin_port);
        printf("IP: %s\nPORT: %d", ipBuffer, clientPort);

        if (logConnection(__DATE__, __TIME__, query, "GET", ipBuffer, clientPort) == 0) {
            printf("error logging connection!!!");
        }

        char *response;
        char *starter = HTTPSuccess; 
        char* full_query = concatenate(FileStarter, query);
        
        if (strcmp(query, "/Connection.json") == 0) {
            starter = ConnectionsReq;
            full_query = connectionsPath;
        } else if (verifyFilePath(full_query) == 0) {
            full_query = FileNotFoundPath;
            starter = HTTPError;
        }

        response = compileResponse(starter, full_query);

        printf("=========================== Response ================================\n\n");
        // printf("%s", response);

        write_to_socket(client, response, strlen(response)); 

        close(client); 
    }
}

int main(int argc, char const *argv[]) {
    run_server();

    // int success = logConnection(__DATE__, __TIME__, "/index.html", "GET");
    // char *fp = fileContents(connectionsPath, NULL);
    // setNumConnections(fp, strlen(fp));
    // printf("JSON File After: \n%s\n", fileContents(connectionsPath, NULL));

    return 0;
}






