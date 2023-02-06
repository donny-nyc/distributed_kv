#include "hashmap.h"

#include <stdio.h>

int main(void) {
  hashmap *m = new_hashmap(100);

  printf("new hashmap: %zu\n", m->capacity);

  hash_key key;
  hash_value value;

  key.len = 3;
  key.value = "key";

  value.len = 5;
  value.value = "value";

  insert(m, &key, &value);

  key.len = 4;
  key.value = "keyb";

  value.len = 6;
  value.value = "valueb";

  insert(m, &key, &value);

  hash_entry *e = 0;

  key.len=3;
  key.value="key";
  e = fetch(m, &key);

  printf("key: %s\n", key.value);

  key.len=4;
  key.value="keyb";
  e = fetch(m, &key);

  printf("key: %s\n", key.value);

  free_hashmap(m);

  printf("freed hashmap\n");

  return 0;
}
