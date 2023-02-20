#ifndef DATA_STORE_CONTROLLER_H
#define DATA_STORE_CONTROLLER_H

#include "../handlers/http_handler.h"
#include "../hashmap/hashmap.h"

int get_value(hashmap *data_store, char *key, char **value_buffer, size_t *value_len);
int put_value(hashmap *data_store, char *key, char *value, size_t value_len);
int upsert_value(hashmap *data_store, char *key, char *value, size_t value_len);
int delete_value(hashmap *data_store, char *key);

#endif
