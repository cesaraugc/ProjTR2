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
<<<<<<< HEAD

=======
    
>>>>>>> a789148dae87dac24984dbe1c04736a670f6660b
    printf("Proceed?\n");
    getchar();

    string response = makeRequest(msg);

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


void constroiReferencia(set<string> & result, string response, const char *base) {
    // char buff[1000];
    string buff;
    size_t init_index = 0;

    puts(base);
    while ((init_index = response.find("href=\"", init_index)) != string::npos) {
      buff = response.substr(init_index + 6, response.find('\"', init_index + 6) - (init_index + 6));
      if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos)
      {
        init_index += buff.length() + 1;
        continue;
      }
      cout << buff << '\n';
      init_index += buff.length() + 1;
      result.emplace(buff);
    }
    init_index = 0;
    while ((init_index = response.find("src=\"", init_index)) != string::npos) {
      buff = response.substr(init_index + 5, response.find('\"', init_index + 5) - (init_index + 5));
      if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos)
      {
        init_index += buff.length() + 1;
        continue;
      }
      cout << buff << '\n';
      init_index += buff.length() + 1;
      result.emplace(buff);
    }
    init_index = 0;
    while ((init_index = response.find("url(\"", init_index)) != string::npos) {
      buff = response.substr(init_index + 5, response.find('\"', init_index + 5) - (init_index + 5));
      if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos)
      {
        init_index += buff.length() + 1;
        continue;
      }
      cout << buff << '\n';
      init_index += buff.length() + 1;
      result.emplace(buff);
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
