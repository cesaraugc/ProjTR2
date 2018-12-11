#include "connection.hpp"
#include "spyder.hpp"
using namespace std;


set<string> spyder(string baseURL){
    /**
    Função principal do spyder.
    @param baseURL: URL base do domínio desejado.
    @return set<string>: Set com os nomes das referências encontradas. 
    */
    
    int levels;
    cout << " Quantos níveis deseja buscar? ";
    cin >> levels;
    cout << endl;

    Tree arvore = generateTree(baseURL, levels);
    arvore.printTree();

    set<string> retr = arvore.treeToVector();
    return retr;
}


void buildReference(set<string> & result, string response, string baseURL) {
    /**
    <Insere em result os arquivos/diretórios encontrados em response>
    @param result: Endereço do set onde serão inseridas as referências.
    @param response: resposta obtida do request.
    @param baseURL: URL base do domínio desejado.
    */

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
        if(buff.empty()) {
            init_index += leng + 1;
            continue;
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
        if(buff.empty()) {
            init_index += leng + 1;
            continue;
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
        if(buff.empty()) {
            init_index += leng + 1;
            continue;
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


bool isHTML(string url, string baseURL) {
    /**
    Inspeciona um cabeçalho para saber se um caminho é HTML para saber se deve ser inspecionado.
    @param url: Url da referência.
    @param baseURL: URL base do domínio desejado.
    @return bool: Indica se é ou não HTML.
    */

    vector<unsigned char> headResponse;
    string head;

    string headMsg = "HEAD " + (url) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    headResponse = makeRequest(headMsg);

    string response_str;
    for(auto letter:headResponse){
        response_str += letter;
    }

    if(response_str.find("text/html") != string::npos && response_str.find("200 OK") != string::npos){
        cout << "\n\tInside isHTML func " << url << " is html" << endl;
        return true;
    } else {
        return false;
    }
}


bool isReallyHTML(string url, string baseURL) {
    /**
    <Verifica se o cabeçalho da url já foi inspecionado para retornar a informação 
    se a url é ou não um HTML.>
    @param url: Url da referência.
    @param baseURL: URL base do domínio desejado.
    @return bool: Indica se é ou não HTML.
    */

    if(mapHTML.find(url) != mapHTML.end()){
        cout << "\nALREADY HAVE " << url << endl;
        return mapHTML[url];
    } else {
        mapHTML[url] = isHTML(url, baseURL);
        return mapHTML[url];        
    }    
}


/* Return all sources of a sources's sons */
set<string> searchChildren(string url, string baseURL){
    /**
    <Busca pelas referências um nível exatamente abaixo da url.>
    @param url: Url que será buscada.
    @param baseURL: URL base do domínio desejado.
    @return set<string>: Conjunto de referências encontradas na url.
    */

    set<string> result;
    if(isReallyHTML(url, baseURL)) {
        cout << endl << "Inspecionando " << url << endl;
        string msg = "GET " + (url) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    
        vector<unsigned char> response = makeRequest(msg); // returning response.txt
        /* Converts a vector of unsigned char to a string */
        string response_str;
        for(auto letter:response){
            response_str += letter;
        }
        result.clear();
        buildReference(result, response_str, baseURL);
    }
    return result;
}


/* Generates the tree */
Tree generateTree(string baseURL, int levels){
    /**
    Gera a árvore hipertextual.
    @param baseURL: URL base do domínio desejado.
    @param levels: Número de níveis máximos desejados para a árvore..
    @return Tree: Árvore gerada.
    */

    set <string> root, visited, visited_tree;
    Tree arvore;

    string msg = "GET http://" + baseURL + "/ HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    cout << msg << "Proceed?" << endl;
    getchar();

    root = searchChildren("/", baseURL);
    
    Node node_to_insert;
    node_to_insert.src = "/";
    node_to_insert.pai = "\0"; // NULL
    if (levels>0)
        node_to_insert.filhos = root;
    node_to_insert.profundidade = 0;
    node_to_insert.isHTML = true;
    arvore.nodes.push_back(node_to_insert);
    
    visited.clear();
    visited.insert("/");
    Node node_to_search;
    int profundidade_arvore = 0;
    while(levels>profundidade_arvore){
        auto arvore2 = arvore;
        for(Node node_to_search:arvore2.nodes)
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

                    result = searchChildren(filho, baseURL);
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
                
                arvore.nodes.push_back(node_to_insert);
                cont++;
                /* Se é o último filho do laço */
                if(cont==((int)node_to_search.filhos.size())){
                    profundidade_arvore = node_to_search.profundidade + 1;
                }
                
            } 
            visited_tree.emplace(node_to_search.src);
        }
        if(arvore2.nodes.size() == arvore.nodes.size()){
            break;
        }
    }
    
    cout << endl;
    
    arvore.depth = profundidade_arvore;

    return arvore;
}
