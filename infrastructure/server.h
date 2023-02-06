#ifndef SERVER
#define SERVER

#include <unistd.h>
#include "../hashmap/hashmap.h"

void start_server(int socket_fd, hashmap* data_store);

#endif
