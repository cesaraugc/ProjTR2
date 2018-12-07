#include "connection.hpp"

int createNewSocket (uint16_t portNum, uint16_t parallelConnections) {
  extern struct freeMemoryList fml;
  struct sockaddr_in *serv; /* socket info about our server */
  int sckt;            /* socket used to listen for incoming connections */

  if ((serv = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in))) == NULL) {
    printf("Erro de alocacao. Abortando\n");
    freeMemory();
    exit(0);
  }
  fml.serv = serv;

  memset(serv, 0, sizeof(struct sockaddr_in));/* zero the struct before filling the fields */
  serv->sin_family = AF_INET;                /* set the type of connection to TCP/IP */
  serv->sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
  serv->sin_port = htons(portNum);           /* set the server port number */

  sckt = socket(AF_INET, SOCK_STREAM, 0);

  /* bind serv information to mysocket */
  bind(sckt, (struct sockaddr *)serv, sizeof(struct sockaddr));

  /* start listening, allowing a queue of up to 1 pending connection */
  listen(sckt, parallelConnections);

  return sckt;
}

void freeMemory () {
  extern struct freeMemoryList fml;
  free(fml.serv);
  free(fml.dest);

  fml.serv = NULL;
  fml.dest = NULL;
}
