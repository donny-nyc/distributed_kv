#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "data_store_controller.h"

hash_key *prepare_hash_key(char *k, size_t key_len) {
  hash_key *key = calloc(1, sizeof(hash_key));

  key->len = key_len;

  key->value = calloc(key_len, sizeof(char));
  strncpy(key->value, k, key_len);

  return key;
}

void free_hash_key(hash_key *key) {
  if(!key) return;

  free(key->value);

  free(key);
}

hash_value *prepare_hash_value(char *k, size_t value_len) {
  hash_value *value = calloc(1, sizeof(hash_value));

  value->value = calloc(value_len, sizeof(char));
  strncpy(value->value, k, value_len);

  value->len = value_len;

  return value;
}

void free_hash_value(hash_value *value) {
  if (!value) return;

  free(value->value);

  free(value);
}

int get_value(hashmap *data_store, char *k, char **value_buffer, size_t *value_len) {
  hash_key *key = prepare_hash_key(k, strlen(k));
  hash_entry *entry = fetch(data_store, key);

  free_hash_key(key);

  printf("entry for %s\n", k);
  if(entry) {
    printf("entry found\n");
    *value_len = entry->pair->value.len;
    *value_buffer = calloc(*value_len, sizeof(char));

    strncpy(*value_buffer, entry->pair->value.value, *value_len);

    return 0;
  }

  printf("[getvalue] not found %s\n", k);

  return E_NOT_FOUND;
}

int put_value(hashmap *data_store, char *k, char *val, size_t val_len) {
  printf("put_value\n");
  hash_key *key = prepare_hash_key(k, strlen(k));

  hash_value *value = prepare_hash_value(val, val_len); 

  insert(data_store, key, value);
  printf("inserted\n");

  free_hash_key(key);
  free_hash_value(value);

  return 0;
}

int upsert_value(hashmap *data_store, char *k, char *val, size_t val_len) {
  printf("put_value\n");
  hash_key *key = prepare_hash_key(k, strlen(k));

  hash_value *value = prepare_hash_value(val, val_len); 

  upsert(data_store, key, value);
  printf("upserted\n");

  free_hash_key(key);
  free_hash_value(value);

  return 0;
}

int delete_value(hashmap *data_store, char *k) {
  printf("delete_value\n");

  hash_key *key = prepare_hash_key(k, strlen(k));

  int res = delete_key(data_store, key);

  free_hash_key(key);

  return res;
}
