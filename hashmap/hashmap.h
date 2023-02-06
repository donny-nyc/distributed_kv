#ifndef HASH_MAP
#define HASH_MAP

#include "hash_entry.h"

struct hash_map_t {
  size_t capacity;
  size_t len;
  hash_entry **entries;
  unsigned int(*hashing_function)(hash_key *);
};

typedef struct hash_map_t hashmap;

int insert(hashmap *, hash_key *, hash_value *);

hash_entry *fetch(hashmap *, hash_key *);

int contains(hashmap *, hash_key *);
//int remove(hashmap *, hash_key *);
//int remove_bulk(hashmap *, hash_key **);

hashmap *new_hashmap(size_t capacity);

int free_hashmap(hashmap *);

#endif
