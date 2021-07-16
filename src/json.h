#include <stdio.h>

//log a server connection to json file
//i.e query = "/index.html"
//i.e request = "GET"
int logConnection(char *date, char *time, char *query, char *request);

//returns num of connections in JSON file or 0 if there is an error
unsigned int getNumConnections(FILE *fp);

//adds one to the number of connections
int setNumConnections(char *contents, int len);