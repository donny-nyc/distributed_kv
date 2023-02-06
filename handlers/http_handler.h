#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <unistd.h>

#include "../hashmap/hashmap.h"

enum http_verb {
  GET,
  POST,
  PUT,
  DELETE,
  UNKNOWN
};

typedef enum http_verb http_verb;

struct http_header_t {
  size_t key_len;
  char *key;
  size_t value_len;
  char *value;
};

typedef struct http_header_t http_header;

struct http_response_t {
  size_t header_len;
  http_header **headers;
  size_t body_len;
  char *body;
};

typedef struct http_response_t http_response;

struct http_request_t {
  //http_response *(handler)(struct http_request_t *self);
  size_t url_len;
  char *url;
  http_verb action;
  size_t header_len;
  http_header **headers;
  size_t body_len;
  char *body;
};

typedef struct http_request_t http_request;

http_response *get(http_request *);
http_response *put(http_request *);
http_response *post(http_request *);
http_response *del(http_request *);

//http_request *parse_request(char *raw, size_t request_len);
//

int handle_request(hashmap *data_store, char *, size_t, char **, size_t *);

#endif
