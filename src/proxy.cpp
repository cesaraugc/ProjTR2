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
        cout << "Erro de alocacao. Abortando\n";
        freeMemory();
        exit(0);
    }
    fml.dest = dest;
    cout << "Aguardando..." << endl;
    int consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
    int len, i = 1000;

    while(--i)
    {
        if((len = read(consocket, rmsg, MAXRCVLEN)) <= 0){
            cout << "Connection close by remote host or some error ocurred. Accepting new connections." << endl;
            close(consocket);
            consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
            continue;
        }
        rmsg[len] = '\0';
        if(strstr(rmsg, "POST") != NULL){
            cout << "I am not accepting POST messages. Plese fuck yourself" << endl;
            close(consocket);
            consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
            continue;
        }
        vector <unsigned char> response = makeRequest(rmsg);
        unsigned char buff[response.size()];
        for(size_t i = 0; i < response.size(); ++i){
            buff[i] = response[i];
        }
        write(consocket, buff, response.size());
        cout << "Done\n";
    }   
  
    close(consocket);
    close(ourSocket);

    freeMemory();
    dest = NULL;

    return EXIT_SUCCESS;
}
