#include "connection.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  int value=0;
  string baseURL;

  cout << "(1) Inspector\n(2) Spyder\n(3) Dump\n\n Escolha uma opção: ";
  while(true){
    scanf("%d", &value);
    if(value== 1 || value == 2 || value == 3) break;
    cout << " Valor incorreto." << endl;
    cout << " Escolha uma opção: ";
  }

  switch (value) {
    case 1:
      inspector();
      break;
    case 2:
      cout << "\n\tForneca um dominio valido:\n\n" << endl;
      cin >> baseURL;
      spyder(baseURL);
      break;  
    case 3:
      makeDump();
      break;
  }
}
