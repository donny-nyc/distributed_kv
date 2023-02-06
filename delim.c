#include <stdio.h>
#include <string.h>
int main(void) {

  char *word, *brkt;

  char test[80];
  char *str = "this is\nnot a\nstring";
  char *sep = "\n";

  strcpy(test, str);

  for(word = strtok_r(test, sep, &brkt); word;word = strtok_r(NULL, sep, &brkt)) {
    printf("word: %s\n",word);
  }

  return 0;
}
