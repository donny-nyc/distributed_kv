#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define THREAD_COUNT 1

typedef struct Task {
  int a, b;
  void (*executor)(int, int);
} Task;

Task taskQueue[256];
int taskCount = 0;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void executeTask(Task *task) {
  task->executor(task->a, task->b);
}

void submitTask(Task task) {
  pthread_mutex_lock(&mutexQueue);
  taskQueue[taskCount] = task;
  taskCount++;

  pthread_mutex_unlock(&mutexQueue);
  pthread_cond_signal(&condQueue);
}

void *startThread(void *args) {
  while(1) {
    Task task;

    pthread_mutex_lock(&mutexQueue);
    while(taskCount == 0) {
      pthread_cond_wait(&condQueue, &mutexQueue);
    }

    task = taskQueue[0];
    int i;
    for (i = 0; i < taskCount -1; i++) {
      taskQueue[i] = taskQueue[i+1];
    }

    taskCount--;

    pthread_mutex_unlock(&mutexQueue);
    executeTask(&task);
  }
}

void sumProd(int a, int b) {
  int sum = a + b;
  int prod = a * b;

  printf("sum: %d, prod: %d\n", sum, prod);
}

int main(int argc, char **argv) {
  pthread_t th[THREAD_COUNT];
  pthread_mutex_init(&mutexQueue, NULL);
  pthread_cond_init(&condQueue, NULL);

  int i;
  for (i = 0; i < THREAD_COUNT; i++) {
    if (pthread_create(&th[i], NULL, &startThread, NULL) != 0) {
      perror("Failed to create a thread.");
    }
  }

  srand(time(NULL));
  for(i = 0; i < 100; i++) {
    Task t = {
      .a = rand() % 100,
      .b = rand() % 100,
      .executor = &sumProd
    };
    submitTask(t);
  }

  for (i = 0; i < THREAD_COUNT; i++) {
    if (pthread_join(th[i], NULL) != 0) {
      perror("Failed to join the thread");
    }
  }

  pthread_mutex_destroy(&mutexQueue);
  pthread_cond_destroy(&condQueue);

  return 0;
}
