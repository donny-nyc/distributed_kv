#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ptr_op(char *x) {
  char *msg =  calloc(strlen(msg), sizeof(char));

  *x = *msg;
}

int main(void) {
  char *x = "old message";

  ptr_op(x);

  printf("message: %s\n", x);

  return 0;
}
