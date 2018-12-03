#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>

struct {
  struct sockaddr_in *dest;
  struct sockaddr_in *serv;
} freeMemoryList;

int createNewSocket(uint16_t, uint16_t);
void freeMemory();
void makeRequest(char*);
void getHostValue(char*, char*);
int inspector();
int spyder();
int dump();
