#include <pthread.h>
#import <stdio.h>
#import <stdlib.h>

#import "list.h"

void list_init(list *l) {
  pthread_mutex_init(&l->lock, NULL);
  pthread_cond_init(&l->condition, NULL);

  l->len = 0;
  l->head = l->tail = 0;
}

void *peek(list *l) {
  if (!l) return 0;
  pthread_mutex_lock(&l->lock);

  while (!l->len) {
    pthread_cond_wait(&l->condition, &l->lock);
  }

  return l->head->entry;

  pthread_mutex_unlock(&l->lock);
}

void *pop(list *l) {

  if (!l) return 0;

  pthread_mutex_lock(&l->lock);

  while (!l->len) {
    pthread_cond_wait(&l->condition, &l->lock);
  }
  
  void *e = l->head->entry;

  list_entry *h = l->head;

  l->len--;
  if(l->head == l->tail) {
    l->head = l->tail = 0;
  } else if (l->head) {
    if(l->head->next) {
      l->head = l->head->next;
      l->head->previous = 0;
    }
  } else {
    printf("shouldn't be here\n");
  }

  free(h);

  pthread_mutex_unlock(&l->lock);

  return e;
}

void push(list *l, void *e) {
  pthread_mutex_lock(&l->lock);

  if (!l) return;

  list_entry *entry = calloc(1, sizeof(list_entry)); 
  entry->entry = e;

  if(l->len == 0) {
    l->head = l->tail = entry;
  } else {
    entry->previous = l->tail;
    l->tail->next = entry;

    l->tail = entry;
  }

  l->len++;

  pthread_mutex_unlock(&l->lock);
  pthread_cond_signal(&l->condition);
}
