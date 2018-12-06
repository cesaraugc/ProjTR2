#include "connection.hpp"

class Node
{ 
    // Access specifier 
    public: 
  
    // Data Members 
    std::string src;
    std::string pai;
    std::set<std::string> filhos;
    int profundidade;
  
    // Member Functions() 
    void printName() 
    { 
       std::cout << src; 
    }

    void printFilhos(){
        int i = profundidade;
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