#include "connection.hpp"
using namespace std;


void makeDump(){
  string baseURL;

  cout << "\n\tForneca um dominio valido:\n\n" << endl;
  cin >> baseURL;
  map<string, set<string>> inspectMap = spyder(baseURL);
  dump(inspectMap, baseURL);  
}

int dump(map<string, set<string>> inspectMap, string baseURL) {

  string request;
  string serverResponse;
  string foldername = "mkdir -p " + baseURL;
  set <string> requests;
  ofstream file;

  system(foldername.c_str());

  for (map<string,set<string>>::iterator it=inspectMap.begin(); it!=inspectMap.end(); ++it){
    for(set<string>::iterator itr=(it->second).begin(); itr!=(it->second).end(); itr++){
      requests.emplace(*itr);
    }    
  }

  for(set<string>::iterator itr = requests.begin(); itr != requests.end(); ++itr){
    if((*itr)[0] != '/'){
      request = "GET /" + (*itr) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n"; 
      foldername = "mkdir -p " + baseURL + "/" + (*itr).substr(0, (*itr).find_last_of('/'));
    } else {
      request = "GET " + (*itr) + " HTTP/1.1\r\nHost: " + baseURL + "\r\nConnection: close\r\n\r\n"; 
      foldername = "mkdir -p " + baseURL + "/" + (*itr).substr(1, (*itr).find_last_of('/'));
    }
    cout << "Inside dump function\n" << request << "Proceed" <<endl;
    serverResponse = makeRequest(request);
    serverResponse = cutHead(serverResponse);

    if(!isHTML(*itr)) {
      system(foldername.c_str());
      if((*itr)[0] != '/'){
        cout << "tryning to write to folder: " << baseURL + "/" + (*itr) << endl;
        file.open(baseURL + string("/") + (*itr), ofstream::binary); 
      } else {
        cout << "tryning to write to folder: " << baseURL + (*itr) << endl;
        file.open(baseURL + (*itr), ofstream::binary);
      }
      if(file.is_open()){
        file << serverResponse;
        file.close();
      } else {
        cout << "Unable to open file" << endl;
      }
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