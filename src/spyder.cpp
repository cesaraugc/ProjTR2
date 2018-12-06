#include "connection.hpp"
using namespace std;

map<string,set<string>> spyder(string baseURL){
    string msg;
    set <string> root, result, visited;
    map<string,set<string>> inspectMap;

    msg = "GET http://" + baseURL + "/ HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
    puts(msg.c_str());
    
    cout << "Proceed?" << endl;
    getchar();

    string response = makeRequest(msg);
    root.clear();
    constroiReferencia(root, response, string(""));
    inspectMap["/"] = root;
    visited.clear();
    visited.insert("/");

    for (set<string>::iterator itr = root.begin(); itr != root.end(); ++itr)
    {   
        auto search = visited.find(*itr);
        if (search != visited.end()) {
            result = buscaFilhos(*itr, baseURL);
            visited.emplace(*itr);
            inspectMap[*itr] = result;
        }
        
        // if(isHTML(*itr)) {
        //     cout << endl << "Inspecionando " << *itr << endl;
        //     msg = "GET " + (*itr) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";

        //     response = makeRequest(msg); // returning response.txt
        //     result2.clear();
        //     constroiReferencia(result2, response, (*itr));
        //     inspectMap[*itr] = result2;
        // }
    }
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
    std::size_t lastValue;

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
        return result;
    }
}