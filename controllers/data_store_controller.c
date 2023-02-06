#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "data_store_controller.h"

int get_value(hashmap *data_store, char *k, char **value_buffer, size_t *value_len) {
  hash_key key;
  key.len = strlen(k);
  key.value = calloc(key.len, sizeof(char));
  strcpy(key.value, k);
  hash_entry *entry = fetch(data_store, &key);


  printf("entry for %s\n", k);
  if(entry) {
    printf("entry found\n");
    *value_len = entry->pair->value.len;
    *value_buffer = calloc(*value_len, sizeof(char));

    strncpy(*value_buffer, entry->pair->value.value, *value_len);

    return 0;
  }

  printf("[getvalue] not found %s\n", key.value);
  free(key.value);

  return -1;
}

int put_value(hashmap *data_store, char *k, char *val, size_t val_len) {
  printf("put_value\n");
  hash_key key;
  key.len = strlen(k);
  key.value = calloc(key.len, sizeof(char));

  printf("strcpy key\n");
  strcpy(key.value, k);

  hash_value value;  
  value.len = val_len;
  value.value = calloc(value.len, sizeof(char));

  printf("val len: %zu\n", value.len);


  if(value.len > 0)
    strcpy(value.value, val);
  printf("strcpy value\n");

  insert(data_store, &key, &value);
  printf("inserted\n");

  free(key.value);

  return 0;
}
