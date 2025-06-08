#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/step.h"
#include "../include/queue.h"
#include "../include/vehicle.h"
#include "../include/common.h"
#include "test_registry.h"

// `current_test_success` i `current_test_name` są globalne i dostępne z test_main.c

void test_direction_val() {
    current_test_success = direction_val("north") == 0 &&
                           direction_val("east") == 2 &&
                           direction_val("south") == 4 &&
                           direction_val("west") == 6 &&
                           direction_val("invalid") == -1;
}

void test_are_colliding_false() {
    Queue *q1 = create_queue("north", TYPE_QUEUE);
    Queue *q2 = create_queue("south", TYPE_QUEUE);
    enqueue(q1, create_vehicle("v1", "south"));
    enqueue(q2, create_vehicle("v2", "north"));
    current_test_success = !are_colliding(q1, q2);
    free_queue_deep(q1);
    free_queue_deep(q2);
}

void test_are_colliding_true() {
    Queue *q1 = create_queue("north", TYPE_QUEUE);
    Queue *q2 = create_queue("east", TYPE_QUEUE);
    enqueue(q1, create_vehicle("v1", "south"));
    enqueue(q2, create_vehicle("v2", "west"));
    current_test_success = are_colliding(q1, q2);
    free_queue_deep(q1);
    free_queue_deep(q2);
}

void test_step_basic() {
    Queue *main_queue = create_queue("main", TYPE_QUEUE);
    Queue *log_queue = create_queue("log", TYPE_QUEUE);

    Queue *north = create_queue("north", TYPE_QUEUE);
    Queue *south = create_queue("south", TYPE_QUEUE);

    enqueue(north, create_vehicle("v1", "south"));
    enqueue(south, create_vehicle("v2", "north"));

    enqueue(main_queue, north);
    enqueue(main_queue, south);

    step(main_queue, log_queue);

    Queue *step_log = (Queue *) dequeue(log_queue);
    current_test_success = step_log->length == 2;

    free_queue_deep(main_queue);
    free_queue_deep(step_log);
    free_queue_deep(log_queue);
}
