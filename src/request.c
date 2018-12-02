#include "connection.h"

// Funcao que realiza requisicao com um arquivo 
void makeRequest(FILE* fd) {
    struct sockaddr_in dest;
    FILE* fd1;
    struct hostent *hp;
    char msg[500], buffer[500 + 1];
    char *buff;
    int someSocket, len;

    if((hp = gethostbyname("www.df.gov.br")) == NULL){
        fprintf(stderr, "Can't get server's address\n");
        freeMemory();
        exit(1);
    }

    while((buff = fgets(msg, 500, fd)) != NULL) {
        snprintf(msg, strlen(buff), "%s%s", buff, "\r\n");
    }
    puts(msg);
    printf("\nProceed?\n");
    getchar();
    someSocket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&dest, 0, sizeof(dest));                   /* zero the struct */
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(hp->h_name);
    // dest.sin_addr.s_addr = htonl(INADDR_LOOPBACK); /* set destination IP number - localhost, 127.0.0.1 */
    dest.sin_port = htons(80);                        /* set destination port number */

    if(connect(someSocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) == -1){
        printf("Can't connect\n");
        freeMemory();
        exit(1);
    }
    write(someSocket, msg, strlen(msg));
    fd1 = fopen("response.txt.save", "w");
    while((len = read(someSocket, buffer, 500) >= 0)){
        buffer[len] = '\0';
        fprintf(fd1, "%s", buffer);        
    }
    fclose(fd1);
}