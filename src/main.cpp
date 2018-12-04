#include "connection.hpp"

int main(int argc, char *argv[])
{
  int value;

  printf("(1) Inspector\n(2) Spyder\n(3) Dump\n\n Escolha uma opção: ");
  scanf("%d", &value);
  switch (value) {
    case 1:
      inspector();
      break;
    case 2:
      spyder();
      break;
    case 3:
      dump();
  }
}
