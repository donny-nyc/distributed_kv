#include "hashing_function.h"

#include <stdio.h>

unsigned int fnv_hash(hash_key *key) {
  printf("fnv hash\n");
  unsigned char *p = (unsigned char *)key->value;
  unsigned int h = 2166136261; // hash offset

  for(int i = 0; i < key->len; ++i) {
    h = (h * 16777619) ^ p[i]; // fnv prime
  }

  printf("hash value: %u\n", h);

  return h;
}

unsigned int supercolliding_hash(hash_key *key) {
  return 0;
}
