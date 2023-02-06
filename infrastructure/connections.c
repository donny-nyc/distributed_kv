#include "connections.h"
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int get_tcp_socket(char *host, char *port) {
  struct addrinfo hints, *res0;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = PF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  char port_str[64];

  int err = getaddrinfo(host, port, &hints, &res0);

  if(err) {
    printf("%s\n", gai_strerror(err));
    exit(1);
  }

  printf("Listening on port %s at %s\n", port, host);

  int s = socket(res0->ai_family, res0->ai_socktype, res0->ai_protocol);

  if(s < 0) {
    perror("failed to open socket.\n");
    exit(1);
  }

  if(bind(s, res0->ai_addr, res0->ai_addrlen) < 0) {
    perror(strerror(errno));
    exit(1);
  }

  if(listen(s, 100) < 0) {
    perror("backlog");
    exit(1);
  }

  freeaddrinfo(res0);

  return s;
}
