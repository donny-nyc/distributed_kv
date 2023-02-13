#ifndef LIST_H
#define LIST_H

#import <unistd.h>
#import <pthread.h>

#define E_LIST_EMPTY 5
#define E_LIST_UNINITIALIZED 4

typedef struct list_entry_t {
  void *entry;
  struct list_entry_t *previous, *next;
} list_entry;

typedef struct list_t {
  pthread_mutex_t lock;
  pthread_cond_t condition;
  size_t len;
  list_entry *head, *tail;
} list;

void list_init(list *);

void *peek(list *);
void *pop(list *);
void push(list *, void *);

#endif
