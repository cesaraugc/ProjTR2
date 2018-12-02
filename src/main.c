#include "connection.h"

#define PORTNUM 8226
#define MAXRCVLEN 500

int main(int argc, char *argv[])
{
  struct sockaddr_in *dest; /* socket info about the machine connecting to us */
  char rmsg[MAXRCVLEN];
  socklen_t socksize = sizeof(struct sockaddr_in);

  int ourSocket = createNewSocket(PORTNUM, 1);
  if ((dest = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in))) == NULL) {
    printf("Erro de alocacao. Abortando\n");
    freeMemory();
    exit(0);
  }
  freeMemoryList.dest = dest;
  int consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);

  int len, i = 5;

  while(--i)
  {
      printf("Incoming connection from %s\n", inet_ntoa(dest->sin_addr));

      // write(consocket, msg, strlen(msg));
      len = read(consocket, rmsg, MAXRCVLEN);
      rmsg[len] = '\0';
      printf("%s\n", rmsg);
      getchar();
      // if (strstr(rmsg, "GET") == NULL) {// returns Bad request
      //   printf("Bad request: \n");
      //   printf("%s\n", rmsg);
      //   char *somemsg;
      //   if(strstr(rmsg, "HTTP/1.1"))
      //     somemsg = "HTTP/1.1 400 BAD REQUEST\r\n\r\n";
      //   else
      //     somemsg = "HTTP/1.0 400 BAD REQUEST\r\n\r\n";
      //   write(consocket, somemsg, strlen(somemsg));
      // } else {
      // }
      close(consocket);
      consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
  }
  close(ourSocket);

  freeMemory();
  dest = NULL;

  return EXIT_SUCCESS;
}
