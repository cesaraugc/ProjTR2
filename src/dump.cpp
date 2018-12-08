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

  system(foldername.c_str());

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

    if(!isHTML(*itr)) {
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
      string name;
      if((*itr).compare("/") == 0){
        name = "index.html";
      } else {
        name = (*itr).substr((*itr).find_last_of("/"));
        name += ".html";
      }
      name = baseURL + "/" + name;
      response_str = fixRefs(response_str);

      cout << "tryning to write to file: " << name << endl;
      file.open(name, ofstream::binary);
      if(file.is_open()){
        file << response_str;
        file.close();
        cout << "Check references now" << endl;
        getchar();
      } else {
        cout << "Unable to open file. Proceed?" << endl;
        getchar();
      }
      // fixRefs() 
    }
  }
  return EXIT_SUCCESS;
}


string cutHead(string serverRequest) {
  string withouthead;
  size_t len;

  if((len = serverRequest.find("\r\n\r\n")) != string::npos) {
    withouthead = serverRequest.substr(serverRequest.find("\r\n\r\n") + 4);
  }
  return withouthead;
}

string fixRefs(string serverResponse) {
  string buff;
  size_t init_index, leng;

  while ((init_index = serverResponse.find("href=\"", init_index)) != string::npos) {
      leng = string("href=\"").length();
      buff = serverResponse.substr(init_index + leng, serverResponse.find('\"', init_index + leng) - (init_index + leng));
      if((leng = buff.find('?')) != string::npos){
        buff = buff.substr(0, leng);
       }
      if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("//") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos ||buff.find("();") != string::npos||buff.find(".html") != string::npos)
      {
        init_index += buff.length() + 1;
        continue;
      }
      if(isHTML(buff)){
        if(buff.compare("/") == 0){ //They are equals
           buff = string("index.html");
           serverResponse.replace(init_index + leng, buff.length(), buff);
         } else {
           buff = buff.substr(buff.find_last_of("/"));
           buff += ".html";
           serverResponse.replace(init_index + leng, buff.length(), buff);
         }
       }
       init_index += buff.length() + 1;
     }
   init_index = 0;
   while ((init_index = serverResponse.find("src=\"", init_index)) != string::npos) {
       leng = string("src=\"").length();
       buff = serverResponse.substr(init_index + leng, serverResponse.find('\"', init_index + leng) - (init_index + leng));
       if((leng = buff.find('?')) != string::npos){
           buff = buff.substr(0, leng);
       }
       if(buff.find("https") != string::npos || buff.find("#") != string::npos ||buff.find("http") != string::npos ||buff.find("") != string::npos ||buff.find("mailto") != string::npos ||buff.find("www") != string::npos ||buff.find("();") != string::npos||buff.find(".html") != string::npos)
       {
           init_index += buff.length() + 1;
           continue;
       }
       if(isHTML(buff)){
         if(buff.compare("/") == 0){ //They are equals
           buff = string("index.html");
           serverResponse.replace(init_index + leng, buff.length(), buff);
         } else {
           buff.substr(buff.find_last_of("/"));
           buff += ".html";
           serverResponse.replace(init_index + leng, buff.length(), buff);
         }
       }
       init_index += buff.length() + 1;
   }

   return serverResponse;
 }