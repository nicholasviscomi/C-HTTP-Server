#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "Colors.h"
#include "FileHelper.h"

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
void read_from_socket(int socket) {
    read(socket , buffer, BUFFSIZE);
    setTerminalColor(WHITE);
    printf("%s\n",buffer);
}

int main(int argc, char const *argv[]) {
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

    int new_socket;
    while(1) {
        setTerminalColor(PURPLE); 
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        
        if ((new_socket = accept(server, (struct sockaddr *)&server_addr, (socklen_t*)&addrlen)) < 0) {
            printf("Error accepting");
            exit(EXIT_FAILURE);
        }

        read_from_socket(new_socket);
        
        char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World!";
        write(new_socket , response , strlen(response));
        setTerminalColor(CYAN);
        printf("------------------message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}













// #include <stdio.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <netinet/in.h>
// #include <string.h>

// #define PORT 8080
// int main(int argc, char const *argv[]) {
    
//     int server_fd, new_socket; long valread;
//     struct sockaddr_in address;
//     int addrlen = sizeof(address);
    
//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("In socket");
//         exit(EXIT_FAILURE);
//     }
    

//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons( PORT );
    
//     memset(address.sin_zero, '\0', sizeof address.sin_zero);
    
    
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
//         perror("In bind");
//         exit(EXIT_FAILURE);
//     }
//     if (listen(server_fd, 10) < 0) {
//         perror("In listen");
//         exit(EXIT_FAILURE);
//     }

//     while(1) {
//         printf("\n+++++++ Waiting for new connection ++++++++\n\n");
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
//             perror("In accept");
//             exit(EXIT_FAILURE);
//         }
        
//         char buffer[30000] = {0};
//         valread = read( new_socket , buffer, 30000);
//         printf("%s\n",buffer );

//         char *response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello World";
//         printf("\n%s\n", response);    


//         write(new_socket , response , strlen(response));
//         printf("------------------Hello message sent-------------------");
//         close(new_socket);
//     }
//     return 0;
// }


// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <sys/types.h>
// #include <sys/time.h>
// #include <sys/ioctl.h>
// #include <signal.h>
// #include <stdarg.h>
// #include <string.h>
// #include <arpa/inet.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <netdb.h>

// #define SERVER_PORT 80
// #define SA struct sockaddr

// #define MAXLINE 4096

// void err_n_die(const char *fmt, ...);

// int main(int argc, char *argv[]) {
//     int sockfd, n;
//     int sendbytes;
//     struct sockaddr_in  servaddr;
//     char sendline[MAXLINE];
//     char recvline[MAXLINE];

//     if (argc != 2) {
//         err_n_die("usage: %s <address>", argv[0]);
//     }

//     if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         err_n_die("error creating the socket");
//     }

//     bzero(&servaddr, sizeof(servaddr));
//     servaddr.sin_family = AF_INET;
//     servaddr.sin_port = htons(SERVER_PORT);

//     if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
//         err_n_die("inet_pton error for: %s", argv[1]);
//     }

//     if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) {
//         err_n_die("connect failed");
//     }

//     sprintf(sendline, "GET / HTTP/1.1\r\n\r\n");
//     sendbytes = strlen(sendline);

//     if (write(sockfd, sendline, sendbytes) != sendbytes) {
//         err_n_die("write error");
//     }

//     memset(recvline, 0, MAXLINE);

//     while ((n = read(sockfd, recvline, MAXLINE-1)) > 0) {
//         printf("%s", recvline);
//     }

//     if (n < 0) {
//         err_n_die("read error");
//     }
    
//     exit(0);
//     return EXIT_SUCCESS;
// }

// void err_n_die(const char *fmt, ...) {
//     int errno_save;
//     va_list ap;

//     errno_save = errno;

//     va_start(ap, fmt);
//     vfprintf(stdout, fmt, ap);
//     fprintf(stdout, "\n");
//     fflush(stdout);

//     if (errno_save != 0) {
//         fprintf(stdout, "(errno = %d)\n", errno_save);
//         strerror(errno_save);
//         fprintf(stdout, "\n");
//         fflush(stdout);
//     }

//     va_end(ap);
//     exit(1);
// }