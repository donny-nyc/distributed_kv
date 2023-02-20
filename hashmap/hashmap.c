#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

#include "hash_entry.h"
#include "hashmap.h"
#include "hashing_function.h"

hashmap *new_hashmap(size_t capacity) {
  hashmap *map = (hashmap *)malloc(sizeof(hashmap));

  if(!map) return 0;

  map->capacity = capacity;
  map->len = 0;

  map->entries = calloc(capacity, sizeof(struct hash_entry_t));  // (hash_entry **) malloc(sizeof(struct hash_entry_t *));

  // bzero(map->entries, map->capacity * sizeof(struct hash_entry_t *));

  map->hashing_function = fnv_hash;

  return map;
}

int free_hashmap(hashmap *map) {
  for(int i = 0; i < map->len; ++i) {
    if(map->entries[i]) {
      free(map->entries[i]);
    }
  }

  free(map);

  return 0;
}

int upsert(hashmap *map, hash_key *key, hash_value *value) {
  printf("upsert\n");
  unsigned int idx = map->hashing_function(key) % map->capacity;

  printf("hash idx: %u\n", idx);


  hash_entry **e = map->entries + idx;
  hash_entry *cur = *e;

  while(1) {

    printf("while 1\n");
    if(cur && cur->next) {
      printf("cur and cur->next\n");

      if (strcmp(cur->pair->key.value, key->value) == 0) {
        printf("match found\n");
        free(cur->pair->value.value);
        free(cur->pair->key.value);
        free(cur->pair);

        break;
      }

      cur = cur->next;
    } else if (cur && !cur->next) {
      printf("cur and not cur->next\n");
      if (strcmp(cur->pair->key.value, key->value) == 0) {
        printf("match found\n");
        free(cur->pair->value.value);
        free(cur->pair->key.value);
        free(cur->pair);

        break;
      }

      cur->next = calloc(1, sizeof(hash_entry));
      cur->next->previous = cur;

      cur = cur->next;
      break; 
    } else { // first element
      printf("else\n");
      cur = calloc(1, sizeof(hash_entry));

      *e = cur;

      break;
    }
  }

  cur->pair = (hash_pair *)malloc(sizeof(hash_pair));

  cur->pair->key.len = key->len;
  cur->pair->key.value = calloc(key->len, sizeof(char));
  strncpy(cur->pair->key.value, key->value, key->len);

  printf("upsert value: %s\n", value->value);

  cur->pair->value.len = value->len;
  cur->pair->value.value = calloc(value->len, sizeof(char));
  strncpy(cur->pair->value.value, value->value, value->len);

  map->entries[idx] = *e;

  printf("entry at %u: %s, %s\n", idx,  map->entries[idx]->pair->key.value, map->entries[idx]->pair->value.value);

  return 0;

}

int insert(hashmap *map, hash_key *key, hash_value *value) {
  printf("insert\n");
  unsigned int idx = map->hashing_function(key) % map->capacity;

  printf("hash idx: %u\n", idx);


  hash_entry **e = map->entries + idx;
  hash_entry *cur = *e;

  while(1) {

    printf("while 1\n");
    if(cur && cur->next) {
      printf("cur and cur->next\n");

      if (strcmp(cur->pair->key.value, key->value) == 0) {
        return E_ALREADY_EXISTS;
      }

      cur = cur->next;
    } else if (cur && !cur->next) {
      printf("cur and not cur->next\n");
      if (strcmp(cur->pair->key.value, key->value) == 0) {
        return E_ALREADY_EXISTS;
      }

      cur->next = calloc(1, sizeof(hash_entry));
      cur->next->previous = cur;

      cur = cur->next;
      break; 
    } else { // first element
      printf("else\n");
      cur = calloc(1, sizeof(hash_entry));

      *e = cur;

      break;
    }
  }

  cur->pair = (hash_pair *)malloc(sizeof(hash_pair));

  cur->pair->key.len = key->len;
  cur->pair->key.value = calloc(key->len, sizeof(char));
  strncpy(cur->pair->key.value, key->value, key->len);

  cur->pair->value.len = value->len;
  cur->pair->value.value = calloc(value->len, sizeof(char));
  strncpy(cur->pair->value.value, value->value, value->len);

  map->entries[idx] = *e;

  printf("entry at %u: %s, %s\n", idx,  map->entries[idx]->pair->key.value, map->entries[idx]->pair->value.value);

  return 0;
}

hash_entry *fetch(hashmap *map, hash_key *key) {
  printf("hashmap: fetch\n");
  unsigned int idx = map->hashing_function(key) % map->capacity;
  // printf("entry at %u: %s, %s\n", idx,  map->entries[idx]->pair->key.value, map->entries[idx]->pair->value.value);

  printf("hash fcnt: %u\n", idx);

  hash_entry *e = map->entries[idx];

  if(!e) {
    printf("[fetch] Not found");
    return 0;
  } else {
    printf("hash entry found\n");
  }

  if (e) {
    if (e->pair) {
      printf("e->pair\n");
    } else {
      printf("e->pair missing\n");
    }
  }
  printf("entry: %s\n", e->pair->value.value);

  while(1) {
    int res = strncmp(e->pair->key.value, key->value, key->len);

    printf("while 1\n");
    if(!res) {
      return e;
    } else if (e && e->next) {
      e = e->next;
    } else {
      return 0;
    }
  }
}

void free_hash_entry(hashmap *map, int idx, hash_entry *e) {
  if (!e->previous && !e->next) {
    printf("all entries dropped.\n");

    map->entries[idx] = 0;
  }

  if(e->previous) {
    e->previous->next = e->next;
  }

  if(e->next) {
    e->next->previous = e->previous;
  }

  free(e->pair->key.value);

  free(e->pair->value.value);

  free(e);
}

int delete_key(hashmap *map, hash_key *key) {
  printf("hashmap: delete\n");
  unsigned int idx = map->hashing_function(key) % map->capacity;

  printf("hash fcnt: %u\n", idx);

  hash_entry *e = map->entries[idx];

  if(!e) {
    printf("[delete] Not found");
    return E_NOT_FOUND;
  } else {
    printf("hash entry found\n");
  }

  while(1) {
    int res = strncmp(e->pair->key.value, key->value, key->len);

    printf("while 1\n");
    if(!res) {
      free_hash_entry(map, idx, e);
      return 0;
    } else if (e && e->next) {
      e = e->next;
    } else {
      return E_NOT_FOUND;
    }
  }
}
