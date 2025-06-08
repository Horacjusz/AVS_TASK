#ifndef STEP_H
#define STEP_H

#include "queue.h"
#include <stdbool.h>

int compute_best_mask(Queue *main_queue, int prev_mask);
int direction_val(const char *direction);
bool are_colliding(Queue *q1, Queue *q2);

#endif
