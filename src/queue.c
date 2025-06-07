#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "vehicle.h"
#include "common.h"

Queue *create_queue(const char *id, ObjectType type) {
    Queue *q = malloc(sizeof(Queue));
    q->type = type;
    strncpy(q->id, id, MAX_ID_LENGTH);
    q->length = 0;
    q->head = q->tail = NULL;
    q->next = NULL;
    q->counter = 0;
    return q;
}

void enqueue(Queue *queue, void *data) {

    QueueNode *node = malloc(sizeof(QueueNode));
    node->data = data;
    node->next = NULL;

    if (!queue->tail) {
        queue->head = queue->tail = node;
    } else {
        queue->tail->next = node;
        queue->tail = node;
    }

    queue->length++;
}

void *dequeue(Queue *queue) {
    if (!queue->head) return NULL;

    QueueNode *node = queue->head;
    void *data = node->data;

    queue->head = node->next;
    if (!queue->head) queue->tail = NULL;
    free(node);
    queue->length--;

    return data;
}

void free_queue(Queue *queue, void (*free_func)(void *)) {
    QueueNode *curr = queue->head;
    while (curr) {
        QueueNode *next = curr->next;
        free_func(curr->data);
        free(curr);
        curr = next;
    }
    free(queue);
}

Queue *find_queue(Queue *list, const char *id) {
    while (list) {
        if (strncmp(list->id, id, MAX_ID_LENGTH) == 0) return list;
        list = list->next;
    }
    return NULL;
}

Queue *find_queue_in_queue(Queue *container, const char *id) {
    QueueNode *curr = container->head;
    while (curr) {
        Queue *maybe_q = (Queue *)curr->data;
        if (*((ObjectType *)maybe_q) == TYPE_QUEUE && strcmp(maybe_q->id, id) == 0) {
            return maybe_q;
        }
        curr = curr->next;
    }
    return NULL;
}


Queue *add_queue_if_missing(Queue **list, const char *id, ObjectType type) {
    Queue *existing = find_queue(*list, id);
    if (existing) return existing;

    Queue *new_q = create_queue(id, type);
    new_q->next = *list;
    *list = new_q;
    return new_q;
}

char *get_queue_string(void *q) {
    Queue *queue = (Queue *)q;

    size_t bufsize = 1024;
    char *buffer = malloc(bufsize);
    if (!buffer) return NULL;

    snprintf(buffer, bufsize, "[QUEUE] id: %s → [", queue->id);

    QueueNode *curr = queue->head;
    while (curr) {
        if (!curr->data) {
            strncat(buffer, "NULL,", bufsize - strlen(buffer) - 1);
        } else {
            ObjectType type = *((ObjectType *)curr->data);
            char *item_str = NULL;

            if (type == TYPE_VEHICLE) {
                item_str = get_vehicle_string(curr->data);
            } else if (type == TYPE_QUEUE) {
                item_str = get_queue_string(curr->data);
            } else {
                item_str = strdup("[UNKNOWN]");
            }

            size_t needed = strlen(buffer) + strlen(item_str) + 3;
            if (needed >= bufsize) {
                bufsize *= 2;
                buffer = realloc(buffer, bufsize);
            }

            strcat(buffer, item_str);
            strcat(buffer, ", ");
            free(item_str);
        }

        curr = curr->next;
    }
    
    size_t len = strlen(buffer);
    if (len >= 3 && buffer[len - 2] == ',' && buffer[len - 1] == ' ') {
        buffer[len - 2] = '\0';
    }

    if (strlen(buffer) + 2 >= bufsize) {
        bufsize += 2;
        buffer = realloc(buffer, bufsize);
    }
    strcat(buffer, "]");

    buffer = realloc(buffer, strlen(buffer) + 1);

    return buffer;
}



void print_queue(const Queue *queue) {
    char *content = get_queue_string((void *)queue);
    printf("%s\n", content);
    free(content);
}


void free_queue_deep(Queue *queue) {
    QueueNode *curr = queue->head;
    while (curr) {
        QueueNode *next = curr->next;
        if (curr->data) {
            ObjectType type = *((ObjectType *)curr->data);
            if (type == TYPE_QUEUE) {
                free_queue_deep((Queue *)curr->data);
            } else if (type == TYPE_VEHICLE) {
                free_vehicle(curr->data);
            }
        }
        free(curr);
        curr = next;
    }
    free(queue);
}