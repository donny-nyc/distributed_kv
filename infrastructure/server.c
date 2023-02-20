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

#include <openssl/ssl.h>
#include <openssl/err.h>

#include "server.h"

#include "../handlers/http_handler.h"

#include "../list/list.h"

#define THREAD_COUNT 100

list task_list;

SSL_CTX *create_context() {
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  method = TLS_server_method();

  ctx = SSL_CTX_new(method);

  if(!ctx) {
    perror("Unable to create SSL Context.");
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}

void configure_context(SSL_CTX *ctx) {
  /* Set the key and cert */
  if (SSL_CTX_use_certificate_file(ctx, "certificate.crt", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    // ERR_print_errors(NULL);
    exit(EXIT_FAILURE);
  } 

  if (SSL_CTX_use_PrivateKey_file(ctx, "private.key", SSL_FILETYPE_PEM) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(EXIT_FAILURE);
  }
}

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

typedef struct request_t {
  int fd;
  SSL *ssl;
  hashmap *ds;
  int req_len;
  char buffer[10000];
} request;

void *handle_connection(void *args) {
  while(1) {
    request *req = (request *)pop(&task_list);

    char *response = 0;
    size_t response_len = 0;

    int res = handle_request(req->ds, req->buffer, req->req_len, &response, &response_len);

    if(res == 0 && response_len && response) {
      // if(send(req->fd, response, response_len - 1, 0) < 0) {
      if(SSL_write(req->ssl, response, response_len - 1) < 0) {
        perror("send");
      }

      printf("free response\n");
      free(response);   
    }

    printf("close new fd\n");
    close(req->fd);

    free(req);
  }
}


void start_server(int socket_fd, hashmap *data_store) {
  list_init(&task_list);

  char s[INET6_ADDRSTRLEN];

  struct sockaddr_storage their_addr;
  socklen_t sin_size;

  printf("starting threads.\n");

  pthread_t threads[THREAD_COUNT];

  for(int i = 0; i < THREAD_COUNT; i++) {
    pthread_create(&threads[i], NULL, &handle_connection, NULL);
  }

  printf("setting up ssl.\n");

  SSL_CTX *ctx;

  ctx = create_context();

  printf("waiting for connections.\n");

  while(1) {
    sin_size = sizeof(their_addr);
    int new_fd = accept(socket_fd, (struct sockaddr *)&their_addr, &sin_size);

    if(new_fd < 0) {
      perror("accept");
      continue;
    }

    SSL *ssl;
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, new_fd);

    inet_ntop(their_addr.ss_family,
        get_in_addr((struct sockaddr *)&their_addr),
        s, sizeof(s));

    printf("received connection from %s\n", s);

    request *req = calloc(1, sizeof(request));

    req->fd = new_fd;
    req->ds = data_store;
    req->ssl = ssl;

    //char buffer[10000];
    //bzero(buffer, 10000);
    
    printf("read ssl\n");
    if(SSL_accept(ssl) <= 0) {
      ERR_print_errors_fp(stderr);
    } else {
      // req->req_len = read(new_fd, req->buffer, 10000);
      req->req_len = SSL_read(ssl, req->buffer, 10000);
      if(req->req_len < 0) {
        perror(strerror(errno));
        exit(1);
      }
      printf("read: %s\n", req->buffer);

      push(&task_list, req);
    }

    /*
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
    */

  }

  for(int i = 0; i < THREAD_COUNT; i++) {
    pthread_join(threads[i], NULL);
  }
  
  perror(strerror(errno));
}
