#include "connection.hpp"
using namespace std;

int spyder(){
    char baseURL[100];
    string msg;
    set <string> result, result2;
    set <set<string>> inspectAll;
    FILE *fd2;

    printf("\n\tForneca um dominio valido:\n\n");
    scanf("%s", baseURL);

    msg = "GET http://" + string(baseURL) + "/ HTTP/1.1\r\nHost: " + string(baseURL) + "\r\nConnection: close\r\n\r\n";
    puts(msg.c_str());
    
    printf("Proceed?\n");
    getchar();

    string response = makeRequest(msg);

    constroiReferencia(result, response, string(""));
    inspectAll.insert(result);
    set<string>::iterator itr;

    for (itr = result.begin(); itr != result.end(); ++itr) {
        cout << *itr << endl;
    }

    for (itr = result.begin(); itr != result.end(); ++itr)
    {
        if(isHTML(*itr)) {
            cout << endl << "Inspecionando " << *itr << endl;
            msg = "GET http://" + (*itr) + "/ HTTP/1.1\r\nHost: " + string(baseURL) + "\r\nConnection: close\r\n\r\n";

            response = makeRequest(msg); // returning response.txt

            constroiReferencia(result2, response, (*itr));
            inspectAll.insert(result2);
        }
    }

    fd2 = fopen("spyderman.txt","w");
    size_t index = 0;
    for(set<string>::iterator i = result.begin(); i != result.end(); ++i){
        cout << *i << "-->"<< endl;        
        fprintf(fd2, "%s-->\n", (*i).c_str());
        if(isHTML(*i)){
            set<set<string>>::iterator itr2 = inspectAll[index];
            for (itr = itr2->begin(); itr != itr2->end(); ++itr)
            {
                fprintf(fd2, "\t%s\n", (*itr).c_str()); 
                cout << '\t' << *itr << endl;   
            }
            index++;
        }
    }
    fclose(fd2);
    cout << endl;

    return EXIT_SUCCESS;
}


/* Insere em result os arquivos/diretórios encontrados em response */
void constroiReferencia(set<string> & result, string response, string base) {
    string buff;
    size_t init_index = 0;
    size_t leng;
    
    while ((init_index = response.find("href=\"", init_index)) != string::npos) {
        leng = string("href=\"").length();
        buff = response.substr(init_index + leng, response.find('\"', init_index + leng) - (init_index + leng));
        if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos)
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
        if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos)
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
        if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos)
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

    while(indexSlash != string::npos)
    {
        lastValue = indexSlash;
        indexSlash = value.find('/', lastValue +1);
    }

    if (value.find('.', lastValue) != string::npos){
        return false;
    } else {
        return true;
    }
}
