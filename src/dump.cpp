#include "connection.hpp"
using namespace std;


void makeDump(string baseURL){
  /**
    Função base de Dump. Ela reliza um spider seguido de um dump. O resultado é colocado na pasta raiz dentro de uma pasta com o nome passado como parâmtro
    @param baseURL: URL base do domínio desejado.
  */
  set<string> inspectSet = spyder(baseURL);
  dump(inspectSet, baseURL);
}


int dump(set<string> requests, string baseURL) {
  /**
    Função reliza de fato o resgate das requisições passadas como parâmetro
    @param requests: Set com requisições que deve ser feitas em baseURL.
    @param baseURL: Endereço base onde as requisições serão feitas. Ex: www.ba.gov.br.
    @return int: Um inteiro indicando sucesso das requisições feitas. Ele pode ser ignorado.
  */

  string request;
  vector <unsigned char> serverResponse;
  string systemCommand;
  string filename;
  string foldername = "mkdir -p " + baseURL;
  ofstream file;
  map <string, string> mapRefs;

  system(foldername.c_str());

  generateMap(mapRefs, requests, baseURL);

  for(set<string>::iterator itr = requests.begin(); itr != requests.end(); ++itr){
    if((*itr)[0] != '/'){
      request = "GET /" + (*itr) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
      if ((*itr).find_last_of('/') == string::npos){
        foldername = baseURL + "/";
      } else {
        foldername = baseURL + "/" + (*itr).substr(0, (*itr).find_last_of('/'));
      }
      systemCommand = "mkdir -p " + foldername;
      filename = baseURL + string("/") + (*itr);
    } else {
      request = "GET " + (*itr) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
      foldername = baseURL + (*itr).substr(0, (*itr).find_last_of('/'));
      systemCommand = "mkdir -p " + foldername;
      filename = baseURL + (*itr);
    }
    cout << "Inside dump function\n" << request << "Proceed" <<endl;
    serverResponse = makeRequest(request);
    string response_str;
    for(auto letter:serverResponse){
        response_str += letter;
    }
    response_str = cutHead(response_str);

    if(!isReallyHTML(*itr, baseURL)) {
      system(systemCommand.c_str());
      cout << "tryning to write to folder: " << foldername << endl;
      file.open(filename, ofstream::binary);
      if(file.is_open()){
        file << response_str;
        file.close();
      } else {
        cout << "Unable to open file. Proceed?" << endl;
        getchar();
      }
    } else {// procurar href, src e trocar as referencias
      string name = mapRefs[(*itr)];
      name = baseURL + "/" + name;
      fixRefs(response_str, mapRefs);//necessario arrumar as referencias

      cout << "\n\tTryning to write to file: " << name << endl;
      file.open(name, ofstream::binary);
      if(file.is_open()){
        file << response_str;
        file.close();
      } else {
        cout << "Unable to open file. Proceed?" << endl;
        getchar();
      }
    }
  }
  return EXIT_SUCCESS;
}


void generateMap(map<string, string> &mapRefs, set<string> &requests, string baseURL) {
  /**
    Função que realiza o mapeamento das referências para as referências corretas que serão colocadas no html depois de baixado.
    @param &mapRefs: Referência para um Map com requisições que devem ser mapeadas.
    @param &requests: Referência para um Set que contêm as requisições que devem ser feitas em baseURL.
    @param baseURL: Endereço base onde as requisições serão feitas. Ex: www.unb.br.
  */

  for(set<string>::iterator i = requests.begin(); i != requests.end(); ++i){
    if(!isReallyHTML(*i, baseURL)){//se nao for html mapeie dele para ele mesmo
      if((*i).front() == '/'){
        mapRefs[*i] = (*i).substr(1);
      } else {
        mapRefs[*i] = *i;
      }
    }
    else if((*i).compare("") == 0){//eh uma referencia quebrada
      continue;
    } else if((*i).compare("/") == 0){//eh a raiz "/" -> index.html
      mapRefs[*i] = "index.html";
    } else if((*i).find("/") == string::npos) {//eh um html e nao possui / -> (nome.html)
      mapRefs[*i] = (*i) + ".html";
    } else if (((*i).find_last_of("/")) != string::npos) {//eh um html e possui /
      string buff = (*i);
      if(buff.back() == '/') {//ultimo caractere eh uma "/" -> substituir por .html
        buff.back() = '.';
        buff += "html";
      } else {
        buff += ".html";
      }
      buff = buff.substr(buff.find_last_of("/") + 1);
      mapRefs[*i] = buff;
    }
  }
}


string cutHead(string serverRequest) {
  /**
    Função que corta o cabeçalho e retorna a stream de dados da resposta do servidor.
    @param serverRequest: String que contém a resposta vinda do servidor.
    @return string: Uma string com os dados.
  */
  string withouthead;
  size_t len;

  if((len = serverRequest.find("\r\n\r\n")) != string::npos) {
    withouthead = serverRequest.substr(serverRequest.find("\r\n\r\n") + 4);
  }
  return withouthead;
}


void fixRefs(string &serverResponse, map<string, string> &mapRefs) {
  /**
    Função que corrige as referências no HTML apontando para o local correto.
    @param &serverResponse: Referência à uma string que contém a reposta do servidor (deve ser um HTML).
    @param &mapRefs: Referência à um Map que contêm as referências corretas.
  */
  string buff;
  size_t init_index = 0, leng, leng2;
  size_t end_index;


  leng = string("href=\"").length();
  while ((init_index = serverResponse.find("href=\"", init_index)) != string::npos) {
    end_index = serverResponse.find('\"', init_index + leng);
    buff = serverResponse.substr(init_index + leng, end_index - (init_index + leng));
    if((leng2 = buff.find('?')) != string::npos){
      buff = buff.substr(0, leng2);
    }
    if(buff.length() >= 2){
      if(buff[0] == '/' && buff[1] == '/') {
        buff = string("http:") + buff;
        serverResponse.replace(init_index + leng, end_index - (init_index + leng), buff);
        continue;
      }
    }
    if(mapRefs.find(buff) != mapRefs.end()){
      serverResponse.replace(init_index + leng, end_index - (init_index + leng), mapRefs[buff]);
    }
    init_index = end_index + 1;
  }
  init_index = 0;

  leng = string("src=\"").length();
  while ((init_index = serverResponse.find("src=\"", init_index)) != string::npos) {
    end_index = serverResponse.find('\"', init_index + leng);
    buff = serverResponse.substr(init_index + leng, end_index - (init_index + leng));
    if((leng2 = buff.find('?')) != string::npos){
      buff = buff.substr(0, leng2);
    }
    if(buff.length() >= 2){
      if(buff[0] == '/' && buff[1] == '/') {
        buff = string("http:") + buff;
        serverResponse.replace(init_index + leng, end_index - (init_index + leng), buff);
        continue;
      }
    }
    if(mapRefs.find(buff) != mapRefs.end()){
      serverResponse.replace(init_index + leng, end_index - (init_index + leng), mapRefs[buff]);
    }
    init_index = end_index + 1;
  }

  init_index = 0;

  leng = string("url(\"").length();
  while ((init_index = serverResponse.find("url(\"", init_index)) != string::npos) {
    end_index = serverResponse.find('\"', init_index + leng);
    buff = serverResponse.substr(init_index + leng, end_index - (init_index + leng));
    if((leng2 = buff.find('?')) != string::npos){
      buff = buff.substr(0, leng2);
    }
    if(buff.length() >= 2){
      if(buff[0] == '/' && buff[1] == '/') {
        buff = string("http:") + buff;
        serverResponse.replace(init_index + leng, end_index - (init_index + leng), buff);
        continue;
      }
    }
    if(mapRefs.find(buff) != mapRefs.end()){
      serverResponse.replace(init_index + leng, end_index - (init_index + leng), mapRefs[buff]);
    }
    init_index = end_index + 1;
  }
}

