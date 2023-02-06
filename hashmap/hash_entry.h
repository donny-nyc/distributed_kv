#ifndef HASH_ENTRY
#define HASH_ENTRY

#include <unistd.h>

struct string_t {
  size_t len;
  char *value;
};

typedef struct string_t hash_key;
typedef struct string_t hash_value;

struct key_values_t {
  hash_key key;
  hash_value value;
};

typedef struct key_values_t hash_pair;

struct hash_entry_t {
  hash_pair *pair;
  struct hash_entry_t *previous, *next;
};

typedef struct hash_entry_t hash_entry;

#endif
