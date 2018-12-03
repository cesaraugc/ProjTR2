#include "connection.hpp"
using namespace std;

int spyder(){
    char baseURL[100];
    char msg[1000];
    char buff[100]; 
    char *temp;
    vector <string> result;

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
    // returning response.txt
    fd = fopen("response.txt", "r");
    while(fgets(msg, 500, fd) != NULL){
        if((temp = strstr(msg, "href=\"")) != NULL){
            temp += strlen("href=\"");
            (*strstr(temp,"\"")) = '\0';
            sscanf(temp, "%s", buff);
            puts(temp);
            getchar();                             
        } else if ((temp = strstr(msg, "src=\"")) != NULL) {
            temp += strlen("src=\"");
            (*strstr(temp,"\"")) = '\0';
            sscanf(temp, "%s", buff);
            puts(temp);
            getchar();
        }
    }
    fclose(fd);
    return EXIT_SUCCESS;
}