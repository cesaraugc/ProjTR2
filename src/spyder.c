#include "connection.h"

int spyder(){
    char baseURL[100];
    char msg[500];

    printf("\n\tForneca um dominio valido:\n\n");
    scanf("%s", baseURL);

    sprintf(msg, "GET http://%s/ HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", baseURL, baseURL);
    puts(msg);
    FILE *fd = fopen("spyderRequest.txt","w");
    fprintf(fd, "%s", msg);
    fclose(fd);
    printf("Proceed?\n");
    getchar();

    makeRequest("spyderRequest.txt");

    return EXIT_SUCCESS;
}