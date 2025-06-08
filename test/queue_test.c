#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/queue.h"
#include "../include/vehicle.h"
#include "../include/common.h"
#include "test_registry.h"

// `current_test_success` i `current_test_name` są globalne i dostępne z test_main.c

void test_enqueue_and_dequeue() {
    Queue *q = create_queue("north", TYPE_QUEUE);
    Vehicle *v1 = create_vehicle("v1", "east");
    Vehicle *v2 = create_vehicle("v2", "west");

    enqueue(q, v1);
    enqueue(q, v2);

    Vehicle *out1 = dequeue(q);
    Vehicle *out2 = dequeue(q);
    Vehicle *out3 = dequeue(q);

    current_test_success = (
        out1 == v1 &&
        out2 == v2 &&
        out3 == NULL &&
        q->length == 0
    );

    free_vehicle(out1);
    free_vehicle(out2);
    free(q);
}

void test_add_queue_if_missing() {
    Queue *list = NULL;
    Queue *q1 = add_queue_if_missing(&list, "a", TYPE_QUEUE);
    Queue *q2 = add_queue_if_missing(&list, "a", TYPE_QUEUE);
    Queue *q3 = add_queue_if_missing(&list, "b", TYPE_QUEUE);

    current_test_success = (
        q1 == q2 &&
        q1 != q3 &&
        strcmp(q1->id, "a") == 0 &&
        strcmp(q3->id, "b") == 0
    );

    free_queue(q3, free);
    free_queue(q1, free);
}

void test_find_queue() {
    Queue *q1 = create_queue("q1", TYPE_QUEUE);
    Queue *q2 = create_queue("q2", TYPE_QUEUE);
    q2->next = q1;

    Queue *found = find_queue(q2, "q1");

    current_test_success = (found == q1);

    free(q1);
    free(q2);
}

void test_get_queue_string() {
    Queue *q = create_queue("testQ", TYPE_QUEUE);
    Vehicle *v1 = create_vehicle("X1", "east");
    Vehicle *v2 = create_vehicle("Y2", "west");

    enqueue(q, v1);
    enqueue(q, v2);

    char *str = get_queue_string(q);
    current_test_success = (
        strstr(str, "[QUEUE] id: testQ → [") != NULL &&
        strstr(str, "[VEHICLE] id: X1 → east") != NULL &&
        strstr(str, "[VEHICLE] id: Y2 → west") != NULL
    );

    free(str);
    free_queue(q, free_vehicle);
}

void test_find_queue_in_queue() {
    Queue *container = create_queue("container", TYPE_QUEUE);
    Queue *inner = create_queue("inner", TYPE_QUEUE);

    enqueue(container, inner);

    Queue *found = find_queue_in_queue(container, "inner");

    current_test_success = (found == inner);

    free_queue(container, free);
}