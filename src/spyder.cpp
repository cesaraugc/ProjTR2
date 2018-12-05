#include "connection.hpp"
using namespace std;

int spyder(){
    char baseURL[100];
    string msg;
    set <string> result, result2;
    FILE *fd2;

    printf("\n\tForneca um dominio valido:\n\n");
    scanf("%s", baseURL);

    msg = "GET http://" + string(baseURL) + "/ HTTP/1.1\r\nHost: " + string(baseURL) + "\r\nConnection: close\r\n\r\n";
    puts(msg.c_str());
    
    printf("Proceed?\n");
    getchar();

    string response = makeRequest(msg);

    // returning response.txt
    // fd = fopen("response.txt", "r");
    // while(fgets(msg, 500, fd) != NULL){
    //     // isHTML(msg);
    //     constroiReferencia(result, msg, "");
    // }
    // fclose(fd);

    constroiReferencia(result, response.c_str(), "");
    set<string>::iterator itr;

    for (itr = result.begin(); itr != result.end(); ++itr) {
        cout << *itr << endl;
    }

    set<string>::iterator itr2;

    for (itr = result.begin(); itr != result.end(); ++itr)
    {
        if(isHTML(*itr)) {
            cout << *itr << " eh html\n\n" << endl;
            msg = "GET http://" + (*itr) + "/ HTTP/1.1\r\nHost: " + string(baseURL) + "\r\nConnection: close\r\n\r\n";

            response = makeRequest(msg); // returning response.txt

            // fd = fopen("response.txt", "r");
            // while(fgets(msg, 4000, fd) != NULL){
            constroiReferencia(result2, response.c_str(), (*itr).c_str());
            // }
        }
    }
    fd2 = fopen("spyderman.txt","w");
    for (itr2 = result2.begin(); itr2 != result2.end(); ++itr2)
    {
        fprintf(fd2, "%s\n", (*itr2).c_str());
        // cout << *itr2 << '\n';
    }
    fclose(fd2);
    cout << endl;

    return EXIT_SUCCESS;
}


void constroiReferencia(set<string> & result, const char *msg, const char *base) {
    char *temp;
    char buff[1000];

    puts(base);
    // getchar();
    strcpy(buff, base);
    /* Localiza o href */
    if((temp = (char*)strstr(msg, "href=\"")) != NULL){
        temp += strlen("href=\"");
        (*strstr(temp,"\"")) = '\0';    /* lê até a próxima aspa*/
        if(strstr(temp, "?")){
            (*strstr(temp, "?")) = '\0';
        }
        sscanf(temp, "%s", buff);
        if(strstr(buff, "https") || strstr(buff, "#") || strstr(buff, "http") || strstr(buff, "//") || strstr(buff, "mailto") || strstr(buff, "\'") || strstr(buff, "www"))
        {
            return;
        }
        result.emplace(buff);                /* insere item único no set */
    } if ((temp = (char*)strstr(msg, "src=\"")) != NULL) { /* Localiza o src */
        temp += strlen("src=\"");
        (*strstr(temp,"\"")) = '\0';    /* lê até a próxima aspa */
        if(strstr(temp, "?")){
            (*strstr(temp, "?")) = '\0';
        }
        sscanf(temp, "%s", buff);
        if(strstr(buff, "https") || strstr(buff, "#") || strstr(buff, "http") || strstr(buff, "//") || strstr(buff, "mailto") || strstr(buff, "\'") || strstr(buff, "www"))
        {
            return;
        }
        result.emplace(buff);   /* insere item único no set */
    }
}

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
