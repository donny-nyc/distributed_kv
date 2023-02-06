#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char t[] = "PUT / HTTP/1.1\n\
Host: 0.0.0.0:9090\n\
User-Agent: curl/7.64.1\n\
Accept: */*";

  char *prev, *prev2;

  char *first = strtok_r(t, "\n", &prev); 

  printf("first: %s\n", first);

  char *buffer = calloc(strlen(first), sizeof(char));

  strncpy(buffer, first, strlen(first));

  char *word;

  for(word = strtok_r(buffer, " ", &prev2); word != NULL; word = strtok_r(NULL, " ", &prev2)) {
    printf("word: %s\n", word);
  }

  char *header;
  for(header = strtok_r(NULL, "\n", &prev); header != NULL; header = strtok_r(NULL, "\n", &prev)) {
     printf("header: %s\n", header);
  }

  char *c = "";
  char *m;

  printf("%s\n", strtok_r(c, ":", &m));
  printf("%s\n", strtok_r(NULL, ":", &m));



  return 0;
}
