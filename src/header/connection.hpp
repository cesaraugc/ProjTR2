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

class Node
{ 
    // Access specifier 
    public: 
  
    // Data Members 
    std::string src;
    std::string pai;
    std::set<std::string> filhos;
    int profundidade;
    bool isHTML;
    bool printed;

    // Member Functions()
    void printName()
    {
       std::cout << src;
    }

    std::string printFilhos(){
        std::string msg = "";
        if(this->isHTML && (!this->printed)){
            for(int i=profundidade; i!=0; i--){
                std::cout << "\t";
                msg += "\t";
            }
            if(pai != "\0"){
                std::cout << (this->pai) << " =>" << std::endl;
                msg += this->pai + " =>\n";
            }
            else{
                std::cout << "/ => " << std::endl;
                msg+= "/ => \n";
            }
            for(std::string it:this->filhos){
                for(int i=profundidade+1; i!=0; i--){
                    std::cout << "\t";
                    msg += "\t";
                }
                std::cout << it << std::endl;
                msg += it+"\n";
            }
            printed = true;
        }
        return msg;
    }

    void printPai(){
        std::cout << pai;
    }
};

int createNewSocket(uint16_t, uint16_t);
void freeMemory();
std::vector <unsigned char> makeRequest(std::string);
std::string getHostValue(std::string);
int inspector(int);
int proxy(int);

std::set<std::string> spyder(std::string);
void constroiReferencia(std::set<std::string> & ,std::string, std::string);
bool isHTML(std::string, std::string);
bool isRealyHTML(std::string, std::string);
std::vector<unsigned char> readBinaryFile(std::string);
std::string readTextFile(std::string);
bool writeFile(std::string, std::vector<unsigned char>);
std::set<std::string> buscaFilhos(std::string, std::string);
std::vector<Node> generateTree(std::string, int);
Node findInTree(std::vector<Node>, std::string);
std::vector<Node> seekLevel(std::vector<Node>, int);
void printTree(std::vector<Node>, int);
std::set<std::string> treeToVector(std::vector<Node>);

void makeDump();
int dump(std::set<std::string>, std::string);
std::string cutHead(std::string);

void fixRefs(std::string &, std::map<std::string, std::string>&);
void generateMap(std::map<std::string, std::string>&, std::set<std::string>&, std::string);