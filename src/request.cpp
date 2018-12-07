#include "connection.hpp"
using namespace std;

/* Recebe uma string e realiza a requisição */
std::string makeRequest(std::string msg_string) {
    struct sockaddr_in dest;
    struct hostent *hp;
    char buff[1001];
    int someSocket, len;
    string response;

    string host_name = getHostValue(msg_string);
    if((hp = gethostbyname(host_name.c_str())) == NULL){
      fprintf(stderr, "Can't get server's address\n");
      freeMemory();
      exit(1);
    }

    someSocket = socket(AF_INET, SOCK_STREAM, 0);

    memset(&dest, 0, sizeof(dest));                   /* zero the struct */
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)hp->h_addr));
    dest.sin_port = htons(80);                        /* set destination port number */

    if(connect(someSocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in)) == -1){
      printf("Can't connect %s\n", inet_ntoa(dest.sin_addr));
      freeMemory();
      exit(1);
    }
    printf("Connect on host: %s\n", inet_ntoa(dest.sin_addr));
    write(someSocket, msg_string.c_str(), msg_string.length());

    while((len = read(someSocket, buff, 1000)) > 0){
      buff[len] = '\0';
      response += string(buff);
    }
    printf("Response received\n");
    close(someSocket);

    return response;
}

/* Retorna o Host Name da mensagem recebida */
std::string getHostValue(std::string msg_string) {
  std::size_t temp, temp2;
  std::string host_name;

  temp = msg_string.find("Host: ");
  temp += strlen("Host: ");
  msg_string.assign( msg_string, temp, string::npos );
  temp2 = msg_string.find("\r\n");
  host_name.assign( msg_string, 0, temp2 );

  return host_name;
}
