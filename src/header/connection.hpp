#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <vector>
#include <iostream>
#include <set>
#include <unordered_set>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <streambuf>
#include <experimental/filesystem>
#include <map>

struct freeMemoryList {
  struct sockaddr_in *dest;
  struct sockaddr_in *serv;
};

int createNewSocket(uint16_t, uint16_t);
void freeMemory();
std::vector <unsigned char> makeRequest(std::string);
std::string getHostValue(std::string);
int inspector(int);
int proxy(int);

std::set<std::string> spyder(std::string);

bool isReallyHTML(std::string, std::string);
std::vector<unsigned char> readBinaryFile(std::string);
std::string readTextFile(std::string);
bool writeFile(std::string, std::vector<unsigned char>);

void makeDump();
int dump(std::set<std::string>, std::string);
std::string cutHead(std::string);

void fixRefs(std::string &, std::map<std::string, std::string>&);
void generateMap(std::map<std::string, std::string>&, std::set<std::string>&, std::string);