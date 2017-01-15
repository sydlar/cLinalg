#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define isEmpty(q) q->front == NULL
#define notEmpty(q) q->front != NULL

pthread_mutex_t print_mutex;
struct queue masterQueue = {.front = NULL, .rear = NULL};

node_t node_init(void* data_ptr){
    node_t new = malloc(sizeof(struct node));
    new->data_ptr = data_ptr;
    new->next = NULL;
    return new;
}

void node_destroy(node_t node){
    free(node);
}

queue_t queue_init(){
    queue_t q = (queue_t)(malloc(sizeof(struct queue)));
    q->front = NULL;
    pthread_mutex_init(&(q->mutex),0);
    return q;
}

void queue_destroy(queue_t q){
    pthread_mutex_destroy(&(q->mutex));

    node_t next,current = q->front;
    while(current != NULL){
        next = current->next;
        node_destroy(current);
        current = next;
    }
    free(q);
}


void enqueue(queue_t q, void* data_ptr){
    node_t newNode = node_init(data_ptr);
    pthread_mutex_lock(&q->mutex);
    if (isEmpty(q)){
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = q->rear->next;
    }
    pthread_mutex_unlock(&q->mutex);
}

void* dequeue(queue_t q){
    if (isEmpty(q)) return NULL;

    pthread_mutex_lock(&q->mutex);

    node_t current = q->front;
    void* data_ptr = current->data_ptr;
    if (current == q->rear){
        node_destroy(current);
        q->front = NULL;
    } else {
        q->front = current->next;;
        node_destroy(current);
    }

    pthread_mutex_unlock(&q->mutex);

    return data_ptr;
}

int is_empty(queue_t q){
    pthread_mutex_lock(&q->mutex);
    int result = q->front == NULL;
    pthread_mutex_unlock(&q->mutex);
    return result;
}


// PRINTING
void print_as_int_node(node_t node){
    int* value = (int*) node->data_ptr;
    printf("%d -> ",*value);
}

void print_as_int_queue(queue_t q){
    node_t current = q->front;
    while(current != NULL){
        print_as_int_node(current);
        current = current->next;
    }
}


struct queue_test_data {
    int a;
    queue_t q;
};

struct queue_test_data* test_init(queue_t q, int a){
    struct queue_test_data* new = (struct queue_test_data*)malloc(sizeof(struct queue_test_data));
    new->a = a;
    new->q = q;
    return new;
}

void* queue_test(void* ptr){
    struct queue_test_data* in = (struct queue_test_data*)ptr;
    for(int i = 1; i <= 3; i++){
       int* x = malloc(sizeof(int));
       *x = i*(in->a);
       //printf("\nin -> %d\n",*x);fflush(stdout);
       
       //pthread_mutex_lock(&print_mutex);
       printf("i");fflush(stdout);
       //pthread_mutex_unlock(&print_mutex);

       enqueue(in->q,x);
    }
    /*pthread_mutex_lock(&print_mutex);
    printf("\nFINISHED IN %d-THREAD: ",in->a);print_as_int_queue(in->q);printf("\n");fflush(stdout);
    pthread_mutex_unlock(&print_mutex);*/

    int* current = (int*)dequeue(in->q);
    while(current != NULL){
        //pthread_mutex_lock(&print_mutex);
        //printf("\nout -> %d\n",*current);fflush(stdout);
        printf("o");fflush(stdout);
        //pthread_mutex_unlock(&print_mutex);
        //print_as_int_queue(in->q);fflush(stdout);
        current = (int*) dequeue(in->q);
        enqueue(&masterQueue,(void*)current);
    }
    pthread_exit(NULL);
}

int main(int argc, char** argv){

    pthread_mutex_init(&print_mutex,0);
    pthread_mutex_init(&(masterQueue.mutex),0);
   
    queue_t q = queue_init();

    int N = 9;
    pthread_t threads[N];
    int base = 1;
    for(int i = 0; i < N; i++){
        struct queue_test_data* data = test_init(q,base);
        base*=10;
        int rc = pthread_create(&threads[i],NULL,queue_test,(void*)data);
        if (rc) printf("RUNTIME ERROR IN PTHREADS");
    }


    for(int i = 0; i < N; i++){
        pthread_join(threads[i],NULL);
    }


    printf("\nREADY FOR WRITING QUEUE:\n");fflush(stdout);
    print_as_int_queue(&masterQueue);

    pthread_mutex_destroy(&print_mutex);
    pthread_exit(NULL);

}
