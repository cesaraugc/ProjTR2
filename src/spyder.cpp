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
bool isHTML(string value, string baseURL) {
    // size_t indexSlash = 1;
    vector<unsigned char> headResponse;
    string head;

    string headMsg = "HEAD " + (value) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    headResponse = makeRequest(headMsg);

    string response_str;
    for(auto letter:headResponse){
        response_str += letter;
    }

    if(response_str.find("text/html") != string::npos && response_str.find("200 OK") != string::npos){
        cout << "\n\tInside isHTML func " << value << " is html" << endl;
        return true;
    } else {
        return false;
    }

    // if(value == "/")
    //     return true;

    // /* Se encontrar "/" */
    // indexSlash = value.find_last_of('/');
    // if( (indexSlash != string::npos) &&  (value.find('.', indexSlash) != string::npos)){
    //     if(value.find(".html") == string::npos){
    //         return false;
    //     }
    // }
    // else if (value.find(".html") == string::npos){
    //     return false;
    // } 
    // return true;
}

map<string, bool> mapHTML;

bool isRealyHTML(string url, string baseURL) {
    if(mapHTML.find(url) != mapHTML.end()){
        cout << "\nALREADY HAVE" << url << endl;
        return mapHTML[url];
    } else {
        mapHTML[url] = isHTML(url, baseURL);
        return mapHTML[url];        
    }    
}

/* Return all sources of a sources's sons */
set<string> buscaFilhos(string url, string baseURL){
    set<string> result;
    if(isRealyHTML(url, baseURL)) {
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
    set <string> root, visited, visited_tree;
    vector<Node> arvore;

    string msg = "GET http://" + baseURL + "/ HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    cout << msg << "Proceed?" << endl;
    getchar();

    root = buscaFilhos("/", baseURL);
    
    Node node_to_insert;
    node_to_insert.src = "/";
    node_to_insert.pai = "\0"; // NULL
    if (levels>0)
        node_to_insert.filhos = root;
    node_to_insert.profundidade = 0;
    node_to_insert.isHTML = true;
    arvore.push_back(node_to_insert);
    
    visited.clear();
    visited.insert("/");
    Node node_to_search;
    int profundidade_arvore = 0;
    while(levels>profundidade_arvore){
        auto arvore2 = arvore;
        for(Node node_to_search:arvore2)
        {   
            // Se já tiver sido visitado na árvore anterior (arvore2 é sempre realocada)
            if ((visited_tree.find(node_to_search.src) != visited_tree.end()) or (node_to_search.isHTML==false)){
                continue;
            }
            int cont=0;
            for (string filho:node_to_search.filhos)
            { 
                // Node no_pai = findInTree(arvore, node_to_search.src);
                Node node_to_insert;
                node_to_insert.src = filho;
                node_to_insert.pai = node_to_search.src;
                node_to_insert.profundidade = (node_to_search.profundidade + 1);
                set<string> result;
                /* Se o filho ainda não foi visitado e pode possuir filhos */
                if(visited.find(filho) == visited.end())
                {
                    // insere na lista de visitados se ainda não estiver lá
                    visited.emplace(filho);

                    result = buscaFilhos(filho, baseURL);
                    node_to_insert.filhos = result;
                    if(result.empty()){
                        node_to_insert.isHTML = false;
                    }
                    else{
                        node_to_insert.isHTML = true;
                    }
                }
                // Se já foi visitado, o nó não irá possuir filhos (pois já foram 
                // listados em algum nó anterior)
                else{
                    node_to_insert.filhos = result; // array vazio
                    node_to_insert.isHTML = false;
                }
                
                arvore.push_back(node_to_insert);
                cont++;
                /* Se é o último filho do laço */
                if(cont==((int)node_to_search.filhos.size())){
                    profundidade_arvore = node_to_search.profundidade + 1;
                }
                
            } 
            visited_tree.emplace(node_to_search.src);
        }
        if(arvore2.size() == arvore.size()){
            break;
        }
    }
    
    cout << endl;
    
    /* TODO */
    printTree(arvore, profundidade_arvore);
    /* TODO Retirar os "" do constroiReferencia. */

    return arvore;
}



/* Finds a specific node in Tree by it's source */
Node findInTree(vector<Node> arvore, string src){
    for(vector<Node>::iterator it=arvore.begin(); it!=arvore.end(); ++it){
        if((*it).src == src){
            return *it;
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
void printTree(vector<Node> arvore, int profundidade){
    vector<Node> nodes;
    string texto;
    ofstream file;
    file.open("arvore_hipertextual.txt");
    int p=0;
    do{
        nodes = seekLevel(arvore, p);
        for(Node i: nodes){
            texto = printFilhos(i,arvore,profundidade);
            file << texto;
        }
        profundidade--; 
        p++;
    }while(profundidade>0);
    file.close();
}


string printFilhos(Node node, vector<Node> arvore, int profundidade){
    string msg = "";
    for(int i=node.profundidade; i!=0; i--){
        cout << "\t";
        msg += "\t";
    }
    if(node.isHTML){
        if(node.filhos.size() >0){
            cout << (node.src) << " =>" << endl;
            msg += node.src + " =>\n";
            for(string it:node.filhos){
                Node filho = findInTree(arvore, it);
                /* se tiver filhos e não for o último nível, será printado em outro momento */
                if((filho.filhos.size() == 0) || (filho.profundidade==profundidade)){
                    for(int i=node.profundidade+1; i!=0; i--){
                        cout << "\t";
                        msg += "\t";
                    }
                    cout << it << endl;
                    msg += it+"\n";
                }
            }
        }
    }
    else{
        cout << node.src << endl;
        msg += node.src + "\n";
    }
        
    return msg;
}


/* Converts a vector of Nodes to a vector of strings */
set<string> treeToVector(vector<Node> arvore){
    set<string> srcSet;
    for(Node i:arvore){
        srcSet.emplace(i.src);
    }
    return srcSet;
}
