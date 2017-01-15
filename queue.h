#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

struct node;

struct node {
    void* data_ptr;
    struct node* next;
};
typedef struct node* node_t;

struct queue {
    node_t front,rear;
    pthread_mutex_t mutex;
};

typedef struct queue* queue_t;




#endif
