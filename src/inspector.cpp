#include "connection.hpp"

#define MAXRCVLEN 2000


struct freeMemoryList fml;
using namespace std;

int inspector(int PORTNUM) {
  struct sockaddr_in *dest; /* socket info about the machine connecting to us */
  char rmsg[MAXRCVLEN];
  socklen_t socksize = sizeof(struct sockaddr_in);
  FILE* fd;
  cout << "Tentando criar socket..." << endl;
  int ourSocket = createNewSocket(PORTNUM, 1);

  if ((dest = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in))) == NULL) {
    printf("Erro de alocacao. Abortando\n");
    freeMemory();
    exit(0);
  }
  fml.dest = dest;
  cout << "Aguardando...";
  int consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
  int len, i = 10;

  while(--i)
  {
    if((len = read(consocket, rmsg, MAXRCVLEN)) <= 0){
      cout << "Connection close by remote host or some error ocurred" << endl;
      break;
    }
    rmsg[len] = '\0';
    // printf("%s\n", rmsg);
    fd = fopen("request.txt","w");
    fprintf(fd, "%s", rmsg);
    fclose(fd);
    system("nano request.txt");
    // getchar();

    string request = readFile("request.txt");
    string response = makeRequest(request);

    writeFile("response.txt", response);
    //Expecting response.txt to contains response from server
    system("nano response.txt");
    fd = fopen("response.txt","r");
    printf("Writing back:\n");
    response = readFile("response.txt");
    write(consocket, response.c_str(), response.length());
    // while(!feof(fd)){
    //   write(consocket, buff, strlen(buff));
    //   fgets(buff, 200, fd);
    // }
    printf("Done\n");
    fclose(fd);
    // close(consocket);
    // consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
  }
  close(consocket);
  close(ourSocket);

  freeMemory();
  dest = NULL;

  return EXIT_SUCCESS;
}


std::string readFile(string path)
{ 
    string result;
    ifstream ifs(path, ios::binary);
    string str(istreambuf_iterator<char>{ifs}, {});

    return str;
}

bool writeFile(string path, string content){
    ofstream file;
    file.open (path, ios::binary);
    file << content;
    file.close();

    return true;
}