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

class Node
{ 
    // Access specifier 
    public: 
  
    // Data Members 
    std::string src;
    Node* pai;
    std::set<std::string> filhos;
    int profundidade;
    bool isHTML;

    // Member Functions()
    void printName()
    {
       std::cout << src;
    }

    void printFilhos(){
        for(int i=profundidade; i!=0; i--){
            std::cout << "\t";
        }
        std::cout << pai << " =>" << std::endl;
        for(std::set<std::string>::iterator it=filhos.begin(); it!=filhos.end(); it++){
            for(int i=profundidade+1; i!=0; i--){
                std::cout << "\t";
            }
            std::cout << *it << std::endl;
        }
    }

    void printPai(){
        std::cout << pai;
    }
};


int createNewSocket(uint16_t, uint16_t);
void freeMemory();
std::string makeRequest(std::string);
std::string getHostValue(std::string);
int inspector();

std::map<std::string,std::set<std::string>> spyder(std::string);
void constroiReferencia(std::set<std::string> & ,std::string, std::string);
bool isHTML(std::string);
std::string readFile(std::string);
bool writeFile(std::string path, std::string content);
void printTree(std::map<std::string, std::set<std::string>>);

void makeDump();
int dump(std::map<std::string, std::set<std::string>>, std::string);
std::string cutHead(std::string);
std::set<std::string> buscaFilhos(std::string, std::string);
std::string fixRefs(std::string);
std::vector<Node> generateTree(std::string);
Node findInTree(std::vector<Node>, std::string);
std::vector<Node> seekLevel(std::vector<Node>, int);