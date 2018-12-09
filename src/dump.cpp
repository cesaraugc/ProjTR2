#include "connection.hpp"
using namespace std;


void makeDump(){
  string baseURL;

  cout << "\n\tForneca um dominio valido:\n\n" << endl;
  cin >> baseURL;
  set<string> inspectSet = spyder(baseURL);
  dump(inspectSet, baseURL);
}

int dump(set<string> requests, string baseURL) {

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
      foldername = baseURL + "/" + (*itr).substr(0, (*itr).find_last_of('/'));
      systemCommand = "mkdir -p " + foldername;
      filename = baseURL + string("/") + (*itr);
    } else {
      request = "GET " + (*itr) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n";
      foldername = baseURL + (*itr).substr(1, (*itr).find_last_of('/'));
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

    if(!isRealyHTML(*itr, baseURL)) {
      system(systemCommand.c_str());
      cout << "tryning to write to folder: " << baseURL + "/" + (*itr) << endl;
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
        cout << "Check references before proceed" << endl;
        getchar();
      } else {
        cout << "Unable to open file. Proceed?" << endl;
        getchar();
      }
    }
  }
  return EXIT_SUCCESS;
}

void generateMap(map<string, string> &mapRefs, set<string> &requests, string baseURL) {

  for(set<string>::iterator i = requests.begin(); i != requests.end(); ++i){
    if(!isRealyHTML(*i, baseURL)){//se nao for html mapeie dele para ele mesmo
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
  string withouthead;
  size_t len;

  if((len = serverRequest.find("\r\n\r\n")) != string::npos) {
    withouthead = serverRequest.substr(serverRequest.find("\r\n\r\n") + 4);
  }
  return withouthead;
}

void fixRefs(string &serverResponse, map<string, string> &mapRefs) {
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
    if(mapRefs.find(buff) != mapRefs.end()){
      serverResponse.replace(init_index + leng, end_index - (init_index + leng), mapRefs[buff]);
    }
    init_index = end_index + 1;
  }
}

