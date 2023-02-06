#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#include "server.h"

#include "../handlers/http_handler.h"

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


void start_server(int socket_fd, hashmap *data_store) {
  char s[INET6_ADDRSTRLEN];

  struct sockaddr_storage their_addr;
  socklen_t sin_size;

  printf("waiting for connections.\n");

  while(1) {
    sin_size = sizeof(their_addr);
    int new_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &sin_size);

    if(new_fd < 0) {
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),
        s, sizeof(s));

    printf("received connection from %s\n", s);

//    if(!fork()) { // child proc
   //   close(socket_fd);

      char buffer[10000];
      bzero(buffer, 10000);

      printf("read from client\n");
      int r = read(new_fd, &buffer, 10000);
      if(r < 0) {
        perror(strerror(errno));
        exit(1);
      }
      printf("read: %s\n", buffer);


      /// parse_request(buffer, strlen(buffer));
      
      char *response = 0;
      size_t response_len = 0;

      int res = handle_request(data_store, buffer, r, &response, &response_len);

      if(res == 0 && response_len && response) {
        if(send(new_fd, response, response_len - 1, 0) < 0) {
          perror("send");
        }

        printf("free response\n");
        free(response);
      }

      printf("close new fd\n");
      close(new_fd);

  }
  
  perror(strerror(errno));
}
