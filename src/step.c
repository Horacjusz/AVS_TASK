// src/step.c
#include "../include/step.h"
#include "../include/queue.h"
#include "../include/vehicle.h"
#include "../include/common.h"
#include <string.h>
#include <stdbool.h>

// Converts direction to numeric code
int direction_val(const char *direction) {
    for (int i = 0; i < DIRECTIONS_SIZE; i++) {
        if (strcmp(direction, directions[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// Returns 1 if two vehicle paths would collide
bool are_colliding(Queue *q1, Queue *q2) {
    int start1 = 2*direction_val(q1->id);
    int end1 = 2*direction_val(((Vehicle *)(q1->head->data))->target_road);
    int start2 = 2*direction_val(q2->id);
    int end2 = 2*direction_val(((Vehicle *)(q2->head->data))->target_road);

    end1++;
    end2++;

    return (start1 == start2 || end1 == end2 ||
            ((min(start1, end1) < min(start2, end2)) && (min(start2, end2) < max(start1, end1)) && (max(start1, end1) < max(start2, end2))) ||
            ((min(start2, end2) < min(start1, end1)) && (min(start1, end1) < max(start2, end2)) && (max(start2, end2) < max(start1, end1))));
}

// Returns bitmask for green lights (each bit = 1 means queue can move)
int compute_best_mask(Queue *main_queue) {
    if (!main_queue) return 0;

    Queue *ready_queues[128];
    int ready_count = 0;

    // Collects queues with waiting vehicles
    QueueNode *curr = main_queue->head;
    while (curr) {
        Queue *q = (Queue *)curr->data;
        if (q && q->type == TYPE_QUEUE && q->length > 0) {
            ready_queues[ready_count++] = q;
            q->counter += 1;
        }
        curr = curr->next;
    }

    // Brute-force evaluation of all conflict-free queue combinations (bitmask)
    int best_mask = 0;
    int best_score = -1;
    int subsets = 1 << ready_count;

    for (int mask = 1; mask < subsets; ++mask) {
        int valid = 1;
        int score = 0;

        // Checks for collisions in current subset
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

        // Scores the configuration based on queue length and wait time
        for (int i = 0; i < ready_count; ++i) {
            if (mask & (1 << i)) {
                score += ready_queues[i]->length + ready_queues[i]->counter;
            }
        }

        // Updates best-scoring mask
        if (score > best_score) {
            best_score = score;
            best_mask = mask;
        }
    }
    
    int new_mask = 0;
    for (int i = 0; i < ready_count; ++i) {
        if (best_mask & (1 << i)) {
            // Pobieramy ID kierunku np. "north"
            const char *id = ready_queues[i]->id;
            int j = direction_val(id);
            // Szukamy pozycji w globalnym directions[]
            new_mask |= (1 << j);
        }
    }

    return new_mask;
}
