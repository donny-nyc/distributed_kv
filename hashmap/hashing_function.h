#ifndef HASHING_FUNCTIONS
#define HASHING_FUNCTIONS

#include "hash_entry.h"

unsigned int fnv_hash(hash_key *key);
unsigned int supercolliding_hash(hash_key *key);

#endif
