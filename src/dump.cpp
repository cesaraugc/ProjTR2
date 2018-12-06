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
  string systemCommand;
  string filename;
  string foldername = "mkdir -p " + baseURL;
  set <string> requests;
  ofstream file;
  size_t index = 0;

  system(foldername.c_str());

  for (map<string,set<string>>::iterator it=inspectMap.begin(); it!=inspectMap.end(); ++it){
    for(set<string>::iterator itr=(it->second).begin(); itr!=(it->second).end(); itr++){
      requests.emplace(*itr);
    }    
  }

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
    serverResponse = cutHead(serverResponse);

    if(!isHTML(*itr)) {
      system(systemCommand.c_str());
      cout << "tryning to write to folder: " << baseURL + "/" + (*itr) << endl;
      file.open(filename, ofstream::binary); 
      if(file.is_open()){
        file << serverResponse;
        file.close();
      } else {
        cout << "Unable to open file. Proceed?" << endl;
        getchar();
      }
    } else {// procurar href, src e trocar as referencias
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

// string fixRefs(string serverResponse) {
//   while((index = serverResponse.find("href\"", index)) != string::npos){
//         leng = string("href=\"").length();
//         size_t size_str = response.find('\"', index + leng) - (index+leng);
        
//         index += (size_str + leng + 1);
//       }
// }