#ifndef STEP_H
#define STEP_H

#include "queue.h"
#include <stdbool.h>

void step(Queue *main_queue, Queue *log_queue);
int direction_val(const char *direction);
bool are_colliding(Queue *q1, Queue *q2);

#endif
