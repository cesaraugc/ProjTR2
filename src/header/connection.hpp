#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <string>
#include <iostream> 
#include <set>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <streambuf>
#include <filesystem>
struct {
  struct sockaddr_in *dest;
  struct sockaddr_in *serv;
} freeMemoryList;
int createNewSocket(uint16_t, uint16_t);
void freeMemory();
std::string makeRequest(std::string);
std::string getHostValue(std::string);
int inspector();
int spyder();
int dump();
void constroiReferencia(std::set<std::string> & ,const char *, const char*);
bool isHTML(std::string);
std::string readFile(std::string);
bool writeFile(std::string path, std::string content);