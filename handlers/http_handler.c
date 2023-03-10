#include "ctype.h"
#include "http_handler.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../controllers/data_store_controller.h"

char *format = "HTTP/1.1 %zu %s\n\
content-length: %zu\n\
content-type: %s\n\
\n\
%s\r\n";

void http_response_str(char **body, unsigned int code, char *content_type, char *msg, char *bod) {
  asprintf(body, format, code, msg, strlen(bod), content_type, bod);
}

char *http_404_not_found = "HTTP/2 404 NOT FOUND\n\
date: Mon, 16 Jan 2023 19:47:40 GMT\n\
server: Apache\n\
vary: IS_SUBREQ,Accept-Encoding,User-Agent\n\
last-modified: Wed, 16 Nov 2022 15:10:11 GMT\n\
etag: \"147a4-5ed97de78066d-gzip\"\n\
accept-ranges: bytes\n\
cache-control: max-age=600\n\
expires: Mon, 16 Jan 2023 19:57:40 GMT\n\
content-encoding: gzip\n\
content-length: 23\n\
content-type: application/json\n\
X-Firefox-Spdy: h2\n\
\n\
{'message': 'not found'}\
\r\n";



char *http_400_bad_request = "HTTP/2 400 BAD REQUEST\n\
date: Mon, 16 Jan 2023 19:47:40 GMT\n\
server: Apache\n\
vary: IS_SUBREQ,Accept-Encoding,User-Agent\n\
last-modified: Wed, 16 Nov 2022 15:10:11 GMT\n\
etag: \"147a4-5ed97de78066d-gzip\"\n\
accept-ranges: bytes\n\
cache-control: max-age=600\n\
expires: Mon, 16 Jan 2023 19:57:40 GMT\n\
content-encoding: gzip\n\
content-length: 21\n\
content-type: application/json\n\
X-Firefox-Spdy: h2\n\
\n\
{'message': 'not ok'}\
\r\n";

char *http_201_created = "HTTP/2 201 CREATED\n\
date: Mon, 16 Jan 2023 19:47:40 GMT\n\
server: Apache\n\
vary: IS_SUBREQ,Accept-Encoding,User-Agent\n\
last-modified: Wed, 16 Nov 2022 15:10:11 GMT\n\
etag: \"147a4-5ed97de78066d-gzip\"\n\
accept-ranges: bytes\n\
cache-control: max-age=600\n\
expires: Mon, 16 Jan 2023 19:57:40 GMT\n\
content-encoding: gzip\n\
content-length: 5\n\
content-type: text/html\n\
X-Firefox-Spdy: h2\n\
\n\
Created\
\r\n";


char *http_200_ok = "HTTP/2 200 OK\n\
date: Mon, 16 Jan 2023 19:47:40 GMT\n\
server: Apache\n\
vary: IS_SUBREQ,Accept-Encoding,User-Agent\n\
last-modified: Wed, 16 Nov 2022 15:10:11 GMT\n\
etag: \"147a4-5ed97de78066d-gzip\"\n\
accept-ranges: bytes\n\
cache-control: max-age=600\n\
expires: Mon, 16 Jan 2023 19:57:40 GMT\n\
content-encoding: gzip\n\
content-length: 13\n\
content-type: text/html\n\
X-Firefox-Spdy: h2\n\
\n\
Hello, World!\
\r\n";

char *dummy_request = "GET / HTTP/2\n\
Host: 0.0.0.0\n\
Accept: */*\n\
\n";


void parse_verb_path(char *raw, size_t request_len, http_request *req) {
  char *sep = " ";

  char *verb, *url, *brk;

  verb = strtok_r(raw, sep, &brk);

  char *p = verb;

  for(; *p; ++p) *p = tolower(*p);

  if(strncmp(verb, "get", 3) == 0) {
    req->action = GET;
  } else if(strncmp(verb, "put", 3) == 0) {
    req->action = PUT;
  } else if(strncmp(verb, "post", 4) == 0) {
    req->action = POST;
  } else if (strncmp(verb, "delete", 6) == 0) {
    req->action = DELETE;
  } else {
    req->action = UNKNOWN;
  }

  url = strtok_r(NULL, sep, &brk);

  req->url = calloc(strlen(url), sizeof(char));

  strncpy(req->url, url, strlen(url));
}



http_request *parse_request(char *raw, size_t request_len) {
  http_request *req = calloc(1, sizeof(struct http_request_t));
  char *sep = "\n";

  char *line, *brkt, *brkt2;

  line = strtok_r(raw, sep, &brkt);

  parse_verb_path(line, strlen(line), req);

  char *buffer;

  while(1) {
    line = strtok_r(NULL, sep, &brkt);

    printf("line: %s\n", line);

    if (strcmp(line, "\r") == 0) {
      printf("break\n");
      break;
    }

    buffer = calloc(strlen(line), sizeof(char));
    strncpy(buffer, line, strlen(line));

    char *header = strtok_r(buffer, ":", &brkt2);
    char *header_value = strtok_r(NULL, ":", &brkt2);

    printf("(header) %s: %s\n", header, header_value);

    free(buffer);
  }

  if(req->action == POST || req->action == PUT) {
    char buf[10000];
    bzero(buf, 10000);
    line = strtok_r(NULL, sep, &brkt);

    while(line) {
      printf("post line: %s\n", line);


      strcpy(buf + strlen(buf), line);
      printf("buf: %s\n", buf);
      line = strtok_r(NULL, sep, &brkt);
    } /*else {
      req->body_len = 0;
      req->body = NULL;
    }*/
    req->body_len = strlen(buf);
    req->body = calloc(req->body_len, sizeof(char));

    strcpy(req->body, buf);
  }

  return req;
}

char *dummy_res = "not found";

int handle_request(hashmap *data_store, char *req, size_t req_len, char **res, size_t *res_len) {
  printf("handle request:\n");
  http_request *request = parse_request(req, req_len);

  switch(request->action) {
  case GET:
    printf("GET \n");
    break;
  case POST:
    printf("POST \n");
    break;
  case PUT:
    printf("PUT \n");
    break;
  case DELETE:
    printf("DELETE \n");
    break;
  case UNKNOWN:
    printf("UNKNOWN \n");
    break;
  }

  if(request->action == GET) {
    int r = get_value(data_store, request->url, res, res_len);

    free(*res);
    if(r == E_NOT_FOUND) {
      printf("Not Found: %s\n", request->url);
      http_response_str(res, 404, "application/json", "NOT FOUND", "{\"message\": \"not found\"}");
      *res_len = strlen(*res);
    } else {

      http_response_str(res, 200, "text/html", "OK", *res);

      *res_len = strlen(*res);

      printf("found (%zu): %s\n", *res_len, *res);
    }
  } else if (request->action == POST) {
    // should only create if it doesn't already exist, otherwise error
    printf("insert\n");
    int r = put_value(data_store, request->url, request->body, request->body_len);

    if (r < 0) return r;

    *res = calloc(strlen(http_201_created), sizeof(char));
    strcpy(*res, http_201_created);
    *res_len = strlen(http_201_created);

    printf("created\n");
  } else if (request->action == PUT) {
    // should replace if exists, otherwise just create it 
    printf("upsert\n");
    int r = upsert_value(data_store, request->url, request->body, request->body_len);

    if (r < 0) return r;

    *res = calloc(strlen(http_201_created), sizeof(char));
    strcpy(*res, http_201_created);
    *res_len = strlen(http_201_created);
  } else if (request->action == DELETE) {
    printf("delete\n"); 

    int r = delete_value(data_store, request->url);

    if (r == E_NOT_FOUND) {
      printf("Not Found: %s\n", request->url);
      http_response_str(res, 404, "application/json", "NOT FOUND", "{\"message\": \"not found\"}");
      *res_len = strlen(*res);
    } else {
      http_response_str(res, 204, "application/json", "DELETED", "");
      *res_len = strlen(*res);
    }
  } else {
    printf("bad request\n");

    *res = calloc(strlen(http_400_bad_request), sizeof(char));
    strcpy(*res, http_400_bad_request);

    *res_len = strlen(http_400_bad_request);
  }

  printf("Response: %s\n", *res);

  if(request && request->body)
    free(request->body);
  if(request && request->url)
    free(request->url);
  if(request)
    free(request);

  return 0;
}
