#include "connection.h"

// Funcao que realiza requisicao com um arquivo
void makeRequest(char *filePath) {
    struct sockaddr_in dest;
    FILE *fd, *fd1;
    struct hostent *hp;
    char msg[1000];
    char buff[201];
    int someSocket, len;

    msg[0] = '\0';
    fd = fopen(filePath,"r");
    while((fgets(buff, 200, fd)) != NULL){
      strcat(msg, buff);
    }
    fclose(fd);

    getHostValue(msg, buff);
    if((hp = gethostbyname(buff)) == NULL){
      fprintf(stderr, "Can't get server's address\n");
      freeMemory();
      exit(1);
    }

    puts(msg);
    printf("\nProceed?\n");
    getchar();
    someSocket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&dest, 0, sizeof(dest));                   /* zero the struct */
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)hp->h_addr));
    // dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1 */
    dest.sin_port = htons(80);                        /* set destination port number */

    if(connect(someSocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) == -1){
      printf("Can't connect %s\n", inet_ntoa(dest.sin_addr));
      freeMemory();
      exit(1);
    }
    printf("Connect on host: %s\n", inet_ntoa(dest.sin_addr));
    write(someSocket, msg, strlen(msg));
    fd1 = fopen("response.txt", "w");
    // Write response on file response.txt
    while((len = read(someSocket, buff, 200)) > 0){
      buff[len] = '\0';
      fprintf(fd1, "%s", buff);
    }
    fclose(fd1);
    printf("Response received\n");
}

void getHostValue(char *string, char *buffer) {
  char *temp;

	temp = strstr(string, "Host: ");
	temp += strlen("Host: ");
	sscanf(temp, "%s", buffer);
}
