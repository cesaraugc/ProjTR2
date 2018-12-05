#include "connection.hpp"
using namespace std;

// Funcao que realiza com a string
std::string makeRequest(string msg_string) {
    struct sockaddr_in dest;
    FILE *fd, *fd1;
    struct hostent *hp;
    char buff[1001];
    // string buff;
    int someSocket, len;
    string response;

    // msg[0] = '\0';
    // fd = fopen(filePath,"r");
    // while((fgets(buff, 200, fd)) != NULL){
    //   strcat(msg, buff);
    // }
    // fclose(fd);

    string host_name = getHostValue(msg_string);
    // gethostbyname(buff.c_str());
    if((hp = gethostbyname(host_name.c_str())) == NULL){
      fprintf(stderr, "Can't get server's address\n");
      freeMemory();
      exit(1);
    }

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
    write(someSocket, msg_string.c_str(), msg_string.length());

    while((len = read(someSocket, buff, 1000)) > 0){
      buff[len] = '\0';
      response += string(buff);
    }
    printf("Response received\n");
    close(someSocket);

    return response;
}

std::string getHostValue(std::string string) {
  std::size_t temp, temp2;
  std::string host_name;

  temp = string.find("Host: ");
  temp += strlen("Host: ");
  string.assign( string, temp, string::npos );
  temp2 = string.find("\r\n");
  host_name.assign( string, 0, temp2 );

  // sscanf(temp, "%s", buffer);
  // buffer = temp;
  
  return host_name;
}
