#include <stdio.h>
#include <string.h>

#include "arc.h"
#include "unarc.h"

int main(int argc, char *argv[]) {
  if (argc != 4) {
    printf("Wrong number of arguments!\n");
    return 1;
  }
  if (strcmp(argv[1], "-a") == 0) {
    return arc(argv[2], argv[3], 0);
  }
  if (strcmp(argv[1], "-u") == 0) {
    return unarc(argv[2], argv[3], 0);
  }
  printf("Wrong argument! Need -a or -u.\n");
  return 0;
}