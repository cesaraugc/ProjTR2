#include "connection.hpp"
using namespace std;


set<string> spyder(string baseURL){
    
    int levels;
    cout << " Quantos níveis deseja buscar? ";
    cin >> levels;
    cout << endl;

    vector<Node> arvore = generateTree(baseURL, levels);
    // printTree(arvore);

    set<string> retr;
    retr = treeToVector(arvore);
    return retr;
}


/* Insere em result os arquivos/diretórios encontrados em response */
void constroiReferencia(set<string> & result, string response, string baseURL) {
    string buff;
    size_t init_index = 0;
    size_t leng;
    int jump;

    vector<string> excludeRefs = {"https", "#", "//", "mailto", "();", "javascript"};
    while ((init_index = response.find("href=\"", init_index)) != string::npos) {
        leng = string("href=\"").length();
        jump = 0;
        buff = response.substr(init_index + leng, response.find('\"', init_index + leng) - (init_index + leng));
        if((leng = buff.find('?')) != string::npos){
            buff = buff.substr(0, leng);
        }
        for(string ref: excludeRefs){
            if(buff.find(ref) != string::npos)
            {
                init_index += buff.length() + 1;
                jump = 1;
                break;
            }
        }
        if(jump) continue;
        if (buff.find("http") != string::npos && buff.find(baseURL) == string::npos)
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
        jump = 0;
        buff = response.substr(init_index + leng, response.find('\"', init_index + leng) - (init_index + leng));
        if((leng = buff.find('?')) != string::npos){
            buff = buff.substr(0, leng);
        }
        for(string ref: excludeRefs){
            if(buff.find(ref) != string::npos)
            {
                init_index += buff.length() + 1;
                jump = 1;
                break;
            }
        }
        if(jump) continue;
        if (buff.find("http") != string::npos && buff.find(baseURL) == string::npos)
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
        jump = 0;
        buff = response.substr(init_index + leng, response.find('\"', init_index + leng) - (init_index + leng));
        if((leng = buff.find('?')) != string::npos){
            buff = buff.substr(0, leng);
        }
        for(string ref: excludeRefs){
            if(buff.find(ref) != string::npos)
            {
                init_index += buff.length() + 1;
                jump = 1;
                break;
            }
        }
        if(jump)continue;
        if (buff.find("http") != string::npos && buff.find(baseURL) == string::npos)
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

    if(value == "/")
        return true;

    /* Se encontrar "/" */
    indexSlash = value.find_last_of('/');
    if( (indexSlash != string::npos) &&  (value.find('.', indexSlash) != string::npos)){
        if(value.find(".html") == string::npos){
            return false;
        }
    }
    else if (value.find(".html") == string::npos){
        return false;
    } 
    return true;
}


/* Return all sources of a sources's sons */
set<string> buscaFilhos(string url, string baseURL){
    set<string> result;
    if(isHTML(url)) {
        cout << endl << "Inspecionando " << url << endl;
        string msg = "GET " + (url) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    
        vector<unsigned char> response = makeRequest(msg); // returning response.txt
        /* Converts a vector of unsigned char to a string */
        string response_str;
        for(auto letter:response){
            response_str += letter;
        }
        result.clear();
        constroiReferencia(result, response_str, baseURL);
    }
    return result;
}



/* Generates the tree */
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
    if (levels>0)
        node_to_insert.filhos = root;
    node_to_insert.profundidade = 0;
    node_to_insert.isHTML = true;
    arvore.push_back(node_to_insert);
    
    visited.clear();
    int cont_arvore = 0;
    Node node_to_search;
    int profundidade_arvore = 0;
    while(cont_arvore<=(int)arvore.size() && levels>0){
        auto arvore2 = arvore;
        for(Node node_to_search:arvore2)
        {
            if (visited.find(node_to_search.src) == visited.end()) 
            {
                visited.insert(node_to_search.src);
                if(node_to_search.isHTML)
                {
                    int cont = 0;
                    for (string itr:node_to_search.filhos)
                    {   
                        /* Se ainda não foi visitado */
                        if (visited.find(itr) == visited.end()) 
                        {
                            set<string> result = buscaFilhos(itr, baseURL);
                            Node node_to_insert;
                            Node no_pai = findInTree(arvore, node_to_search.src);
                            node_to_insert.src = itr;
                            node_to_insert.pai = (&no_pai);
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
                        }
                        cont++;
                        /* Se é o último filho do laço */
                        if(cont==((int)node_to_search.filhos.size())){
                            profundidade_arvore = node_to_search.profundidade + 1;
                            levels--;
                        }
                    }
                }
            }
            cont_arvore++;
        }
    }
    
    cout << endl;
    printTree(arvore, profundidade_arvore);

    return arvore;
}



/* Finds a specific node in Tree by it's source */
Node findInTree(vector<Node> arvore, string src){
    for(Node i:arvore){
        if(i.src == src){
            return i;
        }
    }
    Node j;
    return j;
}


/* Finds all nodes on that level in Tree */
vector<Node> seekLevel(vector<Node> arvore, int level){
    vector<Node> listNodeLevel;
    for(Node i : arvore ){
        if (i.profundidade == level){
            listNodeLevel.push_back(i);
        }
    }
    return listNodeLevel;
}


/* Print the tree */
void printTree(vector<Node> arvore, int niveis){
    vector<Node> nodes;
    for(Node i:arvore){
        i.printed = false;
    }
    string texto;
    ofstream file;
    file.open("arvore_hipertextual.txt");
    int p=0;
    do{
        nodes = seekLevel(arvore, p);
        for(Node i: nodes){
            texto = i.printFilhos();
            file << texto;
        }
        niveis--; 
        p++;
    }while(niveis>0);
    file.close();
}


/* Converts a vector of Nodes to a vector of strings */
set<string> treeToVector(vector<Node> arvore){
    set<string> srcSet;
    for(Node i:arvore){
        srcSet.emplace(i.src);
    }
    return srcSet;
}
