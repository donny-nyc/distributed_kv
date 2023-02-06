#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>

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
      cur = cur->next;
    } else if (cur && !cur->next) {
      printf("cur and not cur->next\n");
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
