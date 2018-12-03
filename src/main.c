#include "connection.h"

int main(int argc, char *argv[])
{
  int value;

  printf("(1) Inspector\n(2) Spyder\n(3) Dump");
  scanf("%d\n", value);
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
