#include "connection.hpp"
using namespace std;

class Node
{ 
    // Access specifier 
    public: 

    // Node(std::string src, std::string pai, std::set<std::string> filhos, int profundidade) :
		//   src(src), pai(pai), filhos(filhos), profundidade(profundidade)
	  // {}
  
    // Data Members 
    std::string src;
    std::string pai;
    std::set<std::string> filhos;
    int profundidade;
    bool isHTML;
};

map<string,set<string>> spyder(string baseURL){
    string msg;
    set <string> root, visited;
    map<string,set<string>> inspectMap;
    vector<Node> arvore;   

    msg = "GET http://" + baseURL + "/ HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    cout << msg;
    cout << "Proceed?" << endl;
    getchar();

    string response = makeRequest(msg);
    root.clear();
    constroiReferencia(root, response, string(""));
    inspectMap["/"] = root;
    
    Node node_to_insert;
    node_to_insert.src = "/";
    node_to_insert.pai = "/0";
    node_to_insert.filhos = root;
    node_to_insert.profundidade = 0;
    node_to_insert.isHTML = true;
    arvore.push_back(node_to_insert);
    
    visited.clear();
    int cont_arvore = 0;
    do{
        auto arvore2 = arvore;
        for(vector<Node>::iterator n=arvore2.begin(); n!=arvore2.end(); ++n){
            Node node_to_search;
            node_to_search.src = n->src;
            node_to_search.pai = n->pai;
            node_to_search.filhos = n->filhos;
            node_to_search.profundidade = n->profundidade;
            node_to_search.isHTML = n->isHTML;
            if ((visited.find(node_to_search.src) == visited.end()) && (node_to_search.isHTML)) {
                visited.insert(node_to_search.src);
                for (set<string>::iterator itr = node_to_search.filhos.begin(); 
                     itr != node_to_search.filhos.end(); 
                     ++itr)
                {   
                    if (visited.find(*itr) == visited.end()) {
                        set<string> result = buscaFilhos(*itr, baseURL);
                        // visited.insert(*itr);
                        inspectMap[*itr] = result;
                        Node node_to_insert;
                        node_to_insert.src = node_to_search.src;
                        node_to_insert.pai = node_to_search.src;
                        node_to_insert.profundidade = (node_to_search.profundidade + 1);
                        if(result.empty()){
                            node_to_insert.filhos = result;
                            node_to_insert.isHTML = false;
                        }
                        else{
                            node_to_insert.filhos = result;
                            node_to_insert.isHTML = true;
                        }
                        arvore.push_back(node_to_insert);
                        cont_arvore = 0;
                    }
                }
            }
            cont_arvore++;
        }
    } while(cont_arvore<=arvore.size()+5);
    cout << endl;

    printTree(inspectMap);

    return inspectMap;

}


/* Insere em result os arquivos/diretórios encontrados em response */
void constroiReferencia(set<string> & result, string response, string base) {
    string buff;
    size_t init_index = 0;
    size_t leng;

    
    while ((init_index = response.find("href=\"", init_index)) != string::npos) {
        leng = string("href=\"").length();
        buff = response.substr(init_index + leng, response.find('\"', init_index + leng) - (init_index + leng));
        if((leng = buff.find('?')) != string::npos){
            buff = buff.substr(0, leng);
        }
        if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos ||buff.find("();") != string::npos||buff.find(".html") != string::npos)
        {
            init_index += buff.length() + 1;
            continue;
        }
        init_index += buff.length() + 1;
        result.emplace(buff);
        }
        init_index = 0;
    while ((init_index = response.find("src=\"", init_index)) != string::npos) {
        leng = string("src=\"").length();
        buff = response.substr(init_index + leng, response.find('\"', init_index + leng) - (init_index + leng));
        if((leng = buff.find('?')) != string::npos){
            buff = buff.substr(0, leng);
        }
        if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos ||buff.find("();") != string::npos||buff.find(".html") != string::npos)
        {
            init_index += buff.length() + 1;
            continue;
        }
        init_index += buff.length() + 1;
        result.emplace(buff);
    }
    init_index = 0;

    while ((init_index = response.find("url(\"", init_index)) != string::npos) {
        leng = string("url(\"").length();
        buff = response.substr(init_index + leng, response.find('\"', init_index + leng) - (init_index + leng));
        if((leng = buff.find("?",0)) != string::npos){
            buff = buff.substr(0, leng);
        }
        if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos||buff.find("();") != string::npos||buff.find(".html") != string::npos)
        {
            init_index += buff.length() + 1;
            continue;
        }
        init_index += buff.length() + 1;
        result.emplace(buff);
    }
}


/* Verifica se um caminho é HTML para saber se deve ser inspecionado */
bool isHTML(string value) {
    std::size_t indexSlash = 1;

    indexSlash = value.find_last_of('/');
    if(indexSlash != string::npos && value.find('.', indexSlash) != string::npos){
        return false;
    } else {
        return true;
    }
}


void printTree(map<string,set<string>> inspectMap){
    ofstream file;
    file.open("arvore_hipertextual.txt");
    for (map<string,set<string>>::iterator it=inspectMap.begin(); it!=inspectMap.end(); ++it){
        if (it->first == "/") {
            cout << "/ =>" << endl;
            file << "/ =>" << endl;
            continue;
        }
        cout << "\t" << it->first << " => " << endl;
        file << "\t" << it->first << " => " << endl;
        for(set<string>::iterator it2=(it->second).begin(); it2!=(it->second).end(); it2++){
            cout << "\t\t" << *it2 << endl;
            file << "\t\t" << *it2 << endl;
        }
    }
    file.close();
}

set<string> buscaFilhos(string url, string baseURL){
    set<string> result;
    if(isHTML(url)) {
        cout << endl << "Inspecionando " << url << endl;
        string msg = "GET " + (url) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";

        string response = makeRequest(msg); // returning response.txt
        result.clear();
        constroiReferencia(result, response, (url));
    }
    return result;
}