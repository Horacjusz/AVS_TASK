#ifndef QUEUE_H
#define QUEUE_H

#include "common.h"

typedef struct QueueNode {
    void *data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    ObjectType type;
    char id[MAX_ID_LENGTH];
    int length;
    QueueNode *head;
    QueueNode *tail;
    struct Queue *next;
    int counter;
} Queue;

Queue *create_queue(const char *id, ObjectType type);
void enqueue(Queue *queue, void *data);
void *dequeue(Queue *queue);
void free_queue(Queue *queue, void (*free_func)(void *));
Queue *find_queue(Queue *list, const char *id);
Queue *find_queue_in_queue(Queue *container, const char *id);
Queue *add_queue_if_missing(Queue **list, const char *id, ObjectType type);
char *get_queue_string(void *q);
void print_queue(const Queue *queue);

void free_queue_deep(Queue *queue);

#endif
