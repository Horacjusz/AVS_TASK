#include "step.h"
#include "queue.h"
#include "vehicle.h"
#include "common.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int direction_val(const char *direction) {
    const char *directions[] = PREDEFINED_DIRECTIONS;
    for (int i = 0; i < DIRECTIONS_SIZE; i++) {
        if (strcmp(direction, directions[i]) == 0) {
            return i * 2;
        }
    }
    return -1;
}

bool are_colliding(Queue *q1, Queue *q2) { 
    int start1 = direction_val(q1->id);
    int end1 = direction_val(((Vehicle *)(q1->head->data))->target_road);
    int start2 = direction_val(q2->id);
    int end2 = direction_val(((Vehicle *)(q2->head->data))->target_road);

    end1++;
    end2++;

    return (start1 == start2 || end1 == end2 ||
           ((min(start1, end1) < min(start2, end2)) && (min(start2, end2) < max(start1, end1)) && (max(start1, end1) < max(start2, end2))) ||
           ((min(start2, end2) < min(start1, end1)) && (min(start1, end1) < max(start2, end2)) && (max(start2, end2) < max(start1, end1))));

}

void step(Queue *main_queue, Queue *log_queue) {
    if (!main_queue || !log_queue) return;

    Queue *left = create_queue("left", TYPE_QUEUE);
    enqueue(log_queue, left);

    Queue *ready_queues[128];
    int ready_count = 0;

    QueueNode *curr = main_queue->head;
    while (curr) {
        Queue *q = (Queue *)curr->data;
        if (q && q->type == TYPE_QUEUE && q->length > 0) {
            ready_queues[ready_count++] = q;
            q->counter += 1;
        }
        curr = curr->next;
    }

    int best_mask = 0;
    int best_score = -1;
    int subsets = 1 << ready_count;

    for (int mask = 1; mask < subsets; ++mask) {
        int valid = 1;
        int score = 0;

        for (int i = 0; i < ready_count && valid; ++i) {
            if (!(mask & (1 << i))) continue;
            for (int j = i + 1; j < ready_count; ++j) {
                if (!(mask & (1 << j))) continue;
                if (are_colliding(ready_queues[i], ready_queues[j])) {
                    valid = 0;
                    break;
                }
            }
        }

        if (!valid) continue;

        for (int i = 0; i < ready_count; ++i) {
            if (mask & (1 << i)) {
                score += ready_queues[i]->length + ready_queues[i]->counter;
            }
        }

        if (score > best_score) {
            best_score = score;
            best_mask = mask;
        }
    }
    
    for (int i = 0; i < ready_count; ++i) {
        if (best_mask & (1 << i)) {
            void *v = dequeue(ready_queues[i]);
            enqueue(left, v);
            ready_queues[i]->counter = 0;
        }
    }

    printf("[STEP] executed, vehicles released:\n");
    print_queue(left);
}
