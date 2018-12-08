#include "connection.hpp"

#define MAXRCVLEN 2000

using namespace std;

int proxy(int PORTNUM) {
    extern struct freeMemoryList fml;
    struct sockaddr_in *dest; /* socket info about the machine connecting to us */
    char rmsg[MAXRCVLEN];
    socklen_t socksize = sizeof(struct sockaddr_in);
    cout << "Tentando criar socket..." << endl;
    int ourSocket = createNewSocket(PORTNUM, 1);

    if ((dest = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in))) == NULL) {
        printf("Erro de alocacao. Abortando\n");
        freeMemory();
        exit(0);
    }
    fml.dest = dest;
    cout << "Aguardando..." << endl;
    int consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
    int len, i = 10;

    while(--i)
    {
        if((len = read(consocket, rmsg, MAXRCVLEN)) == 0){
        cout << "Connection close by remote host" << endl;
        break;
        }
        rmsg[len] = '\0';
        string request = string(rmsg);
        string response = makeRequest(request);
        printf("Done\n");
        close(consocket);
        consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
  }
  close(consocket);
  close(ourSocket);

  freeMemory();
  dest = NULL;

  return EXIT_SUCCESS;
}
