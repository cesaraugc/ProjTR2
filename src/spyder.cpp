#include "connection.hpp"
using namespace std;


set<string> spyder(string baseURL){
    
    int levels;
    cout << "\n\tQuantos níveis deseja buscar? ";
    cin >> levels;
    cout << endl;

    vector<Node> arvore = generateTree(baseURL, levels);
    // printTree(arvore);

    set<string> retr;
    retr = treeToVector(arvore);
    return retr;
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
    size_t indexSlash = 1;

    indexSlash = value.find_last_of('/');
    if(indexSlash != string::npos && value.find('.', indexSlash) != string::npos){
        return false;
    } else {
        return true;
    }
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


vector<Node> generateTree(string baseURL, int levels){
    set <string> root, visited;
    vector<Node> arvore;

    string msg = "GET http://" + baseURL + "/ HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    cout << msg << "Proceed?" << endl;
    getchar();

    root = buscaFilhos("/", baseURL);
    
    Node node_to_insert;
    node_to_insert.src = "/";
    node_to_insert.pai = NULL; // NULL
    node_to_insert.filhos = root;
    node_to_insert.profundidade = 0;
    node_to_insert.isHTML = true;
    arvore.push_back(node_to_insert);
    
    visited.clear();
    int cont_arvore = 0;
    Node node_to_search;
    int profundidade_arvore = 0;
    do{
        auto arvore2 = arvore;
        for(Node n:arvore2){
            node_to_search = n;
            // node_to_search.src = n.src;
            // node_to_search.pai = n.pai;
            // node_to_search.filhos = n.filhos;
            // node_to_search.profundidade = n.profundidade;
            // node_to_search.isHTML = n.isHTML;
            // node_to_search = *n;
            if ((visited.find(node_to_search.src) == visited.end()) && (node_to_search.isHTML)) {
                visited.insert(node_to_search.src);
                for (string itr:node_to_search.filhos)
                {   
                    if (visited.find(itr) == visited.end()) {
                        set<string> result = buscaFilhos(itr, baseURL);
                        Node node_to_insert;
                        Node no_pai = findInTree(arvore, node_to_search.src);
                        node_to_insert.src = (itr);
                        node_to_insert.pai = &no_pai;
                        node_to_insert.filhos = result;
                        node_to_insert.profundidade = (node_to_search.profundidade + 1);
                        if(result.empty()){
                            node_to_insert.isHTML = false;
                        }
                        else{
                            node_to_insert.isHTML = true;
                        }
                        arvore.push_back(node_to_insert);
                        cont_arvore = 0;

                        if(itr == *(node_to_search.filhos.end())){
                            profundidade_arvore = node_to_search.profundidade + 1;
                            levels--;
                        }
                    }
                }
            }
            cont_arvore++;
        }
    } while(cont_arvore<=(int)arvore.size() && levels>0);
    cout << endl;

    printTree(arvore, profundidade_arvore);

    return arvore;
}


Node findInTree(vector<Node> arvore, string src){
    for(Node i :arvore){
        if(i.src == src){
            return i;
        }
    }
    Node j;
    return j;
}


vector<Node> seekLevel(vector<Node> arvore, int level){
    vector<Node> listNodeLevel;
    for(Node i : arvore ){
        if (i.profundidade == level){
            listNodeLevel.push_back(i);
        }
    }
    return listNodeLevel;
}


void printTree(vector<Node> arvore, int niveis){
    vector<Node> Nodes;
    int prof = 0;
    string texto;
    ofstream file;
    file.open("arvore_hipertextual.txt");
    do{
        Nodes = seekLevel(arvore, prof);
        for(Node i: Nodes){
            texto = i.printFilhos();
            file << texto;
        }
        prof++;
    }while(prof<=niveis);
    file.close();
}


set<string> treeToVector(vector<Node> arvore){
    set<string> srcSet;
    for(Node i:arvore){
        srcSet.emplace(i.src);
    }
    return srcSet;
}