#include "connection.h"

#define PORTNUM 8226
#define MAXRCVLEN 1000

int main(int argc, char *argv[])
{
  struct sockaddr_in *dest; /* socket info about the machine connecting to us */
  char rmsg[MAXRCVLEN], buff[200];
  socklen_t socksize = sizeof(struct sockaddr_in);
  FILE* fd;
  int ourSocket = createNewSocket(PORTNUM, 1);

  if ((dest = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in))) == NULL) {
    printf("Erro de alocacao. Abortando\n");
    freeMemory();
    exit(0);
  }
  freeMemoryList.dest = dest;
  int consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);

  int len, i = 10;

  while(--i)
  {
    len = read(consocket, rmsg, MAXRCVLEN);
    rmsg[len] = '\0';
    // printf("%s\n", rmsg);
    fd = fopen("request.txt","w");
    fprintf(fd, "%s", rmsg);
    fclose(fd);
    system("nano request.txt");
    getchar();

    makeRequest("request.txt");
    //Expecting response.txt to contains response from server
    system("nano response.txt");
    fd = fopen("response.txt","r");
    printf("Writing back:\n");
    while((fgets(buff, 200, fd)) != NULL){
      puts(buff);
      write(consocket, buff, strlen(buff));
    }
    printf("Done\n");
    fclose(fd);
    close(consocket);
    consocket = accept(ourSocket, (struct sockaddr *)dest, &socksize);
  }
  close(ourSocket);

  freeMemory();
  dest = NULL;

  return EXIT_SUCCESS;
}
