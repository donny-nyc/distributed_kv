#include "infrastructure/connections.h"
#include "infrastructure/server.h"

#include "hashmap/hashmap.h"

#include <stdio.h>

int main(int argc, char **argv) {
  char *host = argv[1];
  char *port = argv[2];

  hashmap *data_store = new_hashmap(1000);

  if(!data_store) {
    printf("something broke.\n");
    return -1;
  }

  int s = get_tcp_socket(host, port);

  start_server(s, data_store);

  return 0;
}
