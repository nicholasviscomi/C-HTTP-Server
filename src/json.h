#include <stdio.h>

//log a server connection to json file
//i.e query = "/index.html"
//i.e request = "GET"
int logConnection(char *timestamp, char *query, char *request, char *IP, int port);

//returns num of connections in JSON file or 0 if there is an error
unsigned int getNumConnections(FILE *fp);

//adds one to the number of connections
int setNumConnections();